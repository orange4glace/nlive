#include "platform/encoder/encoding_service_impl.h"

#include <iostream>
#include "platform/logger/logger.h"
#include "platform/task/task_service.h"
#include "model/sequence/sequence.h"

namespace nlive {

OutputStream::OutputStream() :
  st(0), enc(0), frame(0), tmp_frame(0), sws_ctx(0), swr_ctx(0),
  next_pts(0), samples_count(0) {}

static OutputStream* add_stream(sptr<Sequence> sequence, OutputStream* ost,
    AVFormatContext* oc, AVCodec** codec, enum AVCodecID codec_id) { 
  AVCodecContext* c = nullptr;
  *codec = avcodec_find_encoder(codec_id);
  if (!(*codec)) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not find encoder for '{}'",
                avcodec_get_name(codec_id));
    return nullptr;
  }
  ost->st = avformat_new_stream(oc, NULL);
  if (!ost->st) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not allocate stream");
    return nullptr;
  }
  ost->st->id = oc->nb_streams - 1;
  c = avcodec_alloc_context3(*codec);
  if (!c) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not alloc an encoding context");
    return nullptr;
  }
  c->thread_count = 0;
  ost->enc = c;

  const AVSampleFormat* supported_fmts = (*codec)->sample_fmts;
  switch((*codec)->type) {
  case AVMEDIA_TYPE_AUDIO:
    while (*supported_fmts != AV_SAMPLE_FMT_NONE) {
      qDebug() << *supported_fmts;
      supported_fmts++;
    }
    // TODO : Check supported format
    c->sample_fmt = AV_SAMPLE_FMT_FLTP;
    c->sample_rate = sequence->sample_rate();
    c->bit_rate = 64000;
    c->channel_layout = AV_CH_LAYOUT_STEREO;
    c->channels = 2;
    ost->st->time_base = {1, c->sample_rate};
    break;
  case AVMEDIA_TYPE_VIDEO:
    c->codec_id = codec_id;
    c->bit_rate = 761000;
    c->width = sequence->width();
    c->height = sequence->height();
    c->time_base = {1 , sequence->base_time()};
    c->gop_size = 12;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
    if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        /* just for testing, we also add B frames */
        c->max_b_frames = 2;
    }
    if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
        /* Needed to avoid using macroblocks in which some coeffs overflow.
          * This does not happen with normal video, it just happens here as
          * the motion of the chroma plane does not match the luma plane. */
        c->mb_decision = 2;
    }
    break;
  default:
    return nullptr;
  }
  /* Some formats want stream headers to be separate. */
  if (oc->oformat->flags & AVFMT_GLOBALHEADER)
      c->flags |= 0x00400000;
  return ost;
}

static AVFrame* alloc_audio_frame(AVSampleFormat sample_fmt,
  uint64_t channel_layout, int sample_rate, int nb_samples) {
  int ret;
  AVFrame* frame = av_frame_alloc();

  if (!frame) {
    spdlog::get(LOGGER_DEFAULT)->error("Error allocating audio frame");
    return nullptr;
  }

  frame->format = sample_fmt;
  frame->channel_layout = channel_layout;
  frame->sample_rate = sample_rate;
  frame->nb_samples = nb_samples;

  if (nb_samples) {
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->error("Error allocating an audio buffer: {}", ret);
      av_frame_free(&frame);
      return nullptr;
    }
  }
  return frame;
}

static AVFrame* alloc_picture(AVPixelFormat pix_fmt, int width, int height) {
  int ret;
  AVFrame* picture;

  picture = av_frame_alloc();
  if (!picture) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not allocate video frame");
    return nullptr;
  }

  picture->format = pix_fmt;
  picture->width = width;
  picture->height = height;

  ret = av_frame_get_buffer(picture, 32);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not allocate video frame data: {}", ret);
    return nullptr;
  }

  return picture;
}

int EncodingTask::openAudio(AVFormatContext* oc, AVCodec* codec,
    OutputStream* ost, AVDictionary* opt_arg) {
  int ret;
  AVCodecContext* c = ost->enc;
  int nb_samples;
  AVDictionary* opt = nullptr;

  av_dict_copy(&opt, opt_arg, 0);
  ret = avcodec_open2(c, codec, &opt);
  av_dict_free(&opt);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not open audio codec: {}", ret);
    return ret;
  }
  if (c->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
    nb_samples = sequence_->sample_rate();
  else
    nb_samples = c->frame_size;
    
  audio_render_context_ = std::make_shared<audio_renderer::RenderContext>(
    AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_FLT, sequence_->sample_rate(), nb_samples);

  ost->frame = alloc_audio_frame(c->sample_fmt, c->channel_layout,
      c->sample_rate, nb_samples);
  if (!ost->frame) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not allocate audio frame: {}", ret);
    return -1;
  }
  // TODO : Calibrate sample format, channel layout, ..
  ost->tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_FLT, c->channel_layout,
      sequence_->sample_rate(), nb_samples);
  if (!ost->tmp_frame) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not allocate tmp audio frame: {}", ret);
    return -1;
  } 

  ret = avcodec_parameters_from_context(ost->st->codecpar, c);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not copy the audio stream parameters");
    return ret;
  }

  ost->swr_ctx = swr_alloc_set_opts(NULL,
    c->channel_layout, c->sample_fmt, c->sample_rate,
    c->channel_layout, AV_SAMPLE_FMT_FLT, sequence_->sample_rate(), 0, 0);
  if (!ost->swr_ctx) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not allocate swrcontext");
    return -1;
  }

  if ((ret = swr_init(ost->swr_ctx)) < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Failed to initialize swrcontext: {}", ret);
    return ret;
  }
  return 0;
}

int EncodingTask::openVideo(AVFormatContext* oc, AVCodec* codec,
    OutputStream* ost, AVDictionary* opt_arg) {
  int ret;
  AVCodecContext* c = ost->enc;
  AVDictionary* opt = nullptr;

  av_dict_copy(&opt, opt_arg, 0);

  ret = avcodec_open2(c, codec, &opt);
  av_dict_free(&opt);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not open video codec: {}", ret);
    return ret;
  }

  ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
  if (!ost->frame) {
    return -1;
  }

  if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
    ost->tmp_frame = alloc_picture(AV_PIX_FMT_YUV420P, c->width, c->height);
    if (!ost->tmp_frame) {
      spdlog::get(LOGGER_DEFAULT)->error("Could not allocate temporary picture");
      return -1;
    }
  }

  ret = avcodec_parameters_from_context(ost->st->codecpar, c);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not copy the video stream parameters");
    return ret;
  }
  return 0;
}

static int writeFrame(AVFormatContext* fmt_ctx, const AVRational* time_base,
    AVStream* st, AVPacket* pkt) {
  av_packet_rescale_ts(pkt, *time_base, st->time_base);
  pkt->stream_index = st->index;
  
  return av_interleaved_write_frame(fmt_ctx, pkt);
}

static int c = 0;
static AVFrame* getAudioFrame(sptr<Sequence> sequence, 
    sptr<audio_renderer::RenderContext> render_context, OutputStream* ost) {
  AVFrame* frame = ost->tmp_frame;
  if (av_compare_ts(
      ost->next_pts, ost->enc->time_base,
      sequence->duration(), { 1, sequence->base_time() }) >= 0) {
    return nullptr;
  }

  int start_frame = ost->next_pts;
  int end_frame = start_frame + frame->nb_samples;
  render_context->clearData();
  auto command_buffer = sequence->renderAudio(start_frame, end_frame);
  for (auto command : command_buffer->commands()) {
    command->render(render_context);
  }

  int32_t *s = (int32_t*)render_context->data();
  int32_t *d = (int32_t*)frame->data[0];
  for (int j = 0; j < frame->nb_samples; j++) {
      for (int i = 0; i < ost->enc->channels; i++)
        *d++ = *s++;
  }

  frame->pts = ost->next_pts;
  ost->next_pts += frame->nb_samples;

  return frame;
}

static int writeAudioFrame(sptr<Sequence> sequence, 
    sptr<audio_renderer::RenderContext> render_context, AVFormatContext* oc,
    OutputStream* ost) {
  int ret;
  int got_packet;
  int dst_nb_samples;
  AVCodecContext* c;
  AVPacket pkt = { 0 };
  AVFrame* frame;

  av_init_packet(&pkt);
  c = ost->enc;

  frame = getAudioFrame(sequence, render_context, ost);

  if (frame) {
    dst_nb_samples = av_rescale_rnd(swr_get_delay(ost->swr_ctx, c->sample_rate) + frame->nb_samples,
        c->sample_rate, c->sample_rate, AV_ROUND_UP);
    assert(dst_nb_samples == frame->nb_samples);

    ret = av_frame_make_writable(ost->frame);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->error("Could not make frame writable: {}", ret);
      return ret;
    }

    ret = swr_convert(ost->swr_ctx, ost->frame->data, dst_nb_samples,
        (const uint8_t**)frame->data, frame->nb_samples);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->error("Error while swr converting: {}", ret);
      return ret;
    }
    frame = ost->frame;

    frame->pts = av_rescale_q(ost->samples_count, {1, c->sample_rate}, c->time_base);
    ost->samples_count += dst_nb_samples;
  }

  ret = avcodec_encode_audio2(c, &pkt, frame, &got_packet);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Error while encoding audio: {}", ret);
    return ret;
  }
  if (got_packet) {
    ret = writeFrame(oc, &c->time_base, ost->st, &pkt);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->error("Error while writing audio frame: {}", ret);
      return ret;
    }
  }

  return (frame || got_packet) ? 0 : 1;
}


void Bitmap2Yuv420p_calc2(uint8_t *destination, uint8_t *rgb, size_t width, size_t height)
{
    size_t image_size = width * height;
    size_t upos = image_size;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line )
    {
        if( !(line % 2) )
        {
            for( size_t x = 0; x < width; x += 2 )
            {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                destination[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                destination[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                r = rgb[3 * i];
                g = rgb[3 * i + 1];
                b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
        else
        {
            for( size_t x = 0; x < width; x += 1 )
            {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
}

static void fill_yuv_image(
    video_renderer::RenderSync* renderer,
    sptr<Sequence> sequence,
    AVFrame* pict, int frame_index, int width, int height) {
  uint8_t* data = new uint8_t[width * height * 3];
  uint8_t* yuv = new uint8_t[width * height + (width * height) / 2];
  auto rb = sequence->renderVideo(frame_index);
  renderer->render(rb);
  auto ctx = renderer->context();
  renderer->readPixels(0, 0, ctx->width(), ctx->height(), GL_RGB, GL_UNSIGNED_BYTE, data);
  Bitmap2Yuv420p_calc2(yuv, data, width, height);
  for (int y = 0; y < height; y ++) {
    for (int x = 0; x < width; x ++) {
      pict->data[0][y * pict->linesize[0] + x] = yuv[y * ctx->width() + x];
    }
  }
  for (int y = 0; y < height / 2; y++) {
      for (int x = 0; x < width / 2; x++) {
          pict->data[1][y * pict->linesize[1] + x] = yuv[ctx->width() * ctx->height() + y * ctx->width() / 2 + x];
          pict->data[2][y * pict->linesize[2] + x] = yuv[ctx->width() * ctx->height() + ctx->width() * ctx->height() / 4 + y * ctx->width() / 2 + x];
      }
  }
  delete data;
  delete yuv;
}

static AVFrame* getVideoFrame(
    video_renderer::RenderSync* renderer,
    sptr<Sequence> sequence,
    OutputStream* ost) {
  int ret;
  AVCodecContext* c = ost->enc;
  if (av_compare_ts(ost->next_pts, c->time_base,
                    sequence->duration(),
                    { sequence->time_base().num(), sequence->time_base().den() }) >= 0) {
    return nullptr;
  }
  ret = av_frame_make_writable(ost->frame);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not make frame writable: {}", ret);
    return nullptr;
  }

  if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
    if (!ost->sws_ctx) {
      ost->sws_ctx = sws_getContext(
        c->width, c->height,
        AV_PIX_FMT_YUV420P,
        c->width, c->height,
        c->pix_fmt,
        SWS_BICUBIC, NULL, NULL, NULL);
      if (!ost->sws_ctx) {
        spdlog::get(LOGGER_DEFAULT)->error("Could not initialize sws_ctx");
        return nullptr;
      }
    }
    fill_yuv_image(renderer, sequence, ost->tmp_frame, ost->next_pts, c->width, c->height);
    sws_scale(ost->sws_ctx,
              (const uint8_t* const*)ost->tmp_frame->data,
              ost->tmp_frame->linesize,
              0, c->height, ost->frame->data, ost->frame->linesize);
  }
  else {
    fill_yuv_image(renderer, sequence, ost->frame, ost->next_pts, c->width, c->height);
  }
  ost->frame->pts = ost->next_pts++;
  return ost->frame;
}

static bool writeVideoFrame(
    video_renderer::RenderSync* renderer,
    sptr<Sequence> sequence,
    AVFormatContext* oc, OutputStream* ost) {
  int ret;
  AVCodecContext* c;
  AVFrame* frame;
  int got_packet = 0;
  AVPacket pkt = { 0 };

  c = ost->enc;

  frame = getVideoFrame(renderer, sequence, ost);
  av_init_packet(&pkt);

  ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Error encoding video frame: {}", ret);
    return true;
  }
  if (got_packet) {
    ret = writeFrame(oc, &c->time_base, ost->st, &pkt);
  }
  else {
    ret = 0;
  }

  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Error while writing video frame: {}", ret);
    return true;
  }

  return !(frame || got_packet);
}

static void closeStream(AVFormatContext* oc, OutputStream* ost) {
  avcodec_free_context(&ost->enc);
  av_frame_free(&ost->frame);
  av_frame_free(&ost->tmp_frame);
  sws_freeContext(ost->sws_ctx);
  swr_free(&ost->swr_ctx);
}

void EncodingTask::run() {

  gl_ctx_ = new QOpenGLContext();
  video_renderer_ = new video_renderer::RenderSync(surface_provider_,
      surface_format_, gl_ctx_, sequence_->width(), sequence_->height());

  AVOutputFormat* fmt;
  AVFormatContext* oc;
  AVCodec *audio_codec, *video_codec;
  AVDictionary* opt = nullptr;
  double audio_time, video_time;
  bool has_video = false, has_audio = false;
  bool encode_video = false, encode_audio = false;
  int ret;
  avformat_alloc_output_context2(&oc, NULL, NULL, std::string(filename_.begin(), filename_.end()).c_str());
  if (!oc) {
    spdlog::get(LOGGER_DEFAULT)->error("Could not deduce output format from file extension: using MPEG.");
    avformat_alloc_output_context2(&oc, NULL, "mpeg", std::string(filename_.begin(), filename_.end()).c_str());
  }
  if (!oc) {
    return;
  }
  fmt = oc->oformat;

  if (fmt->video_codec != AV_CODEC_ID_NONE) {
    add_stream(sequence_, &video_st_, oc, &video_codec, fmt->video_codec);
    has_video = encode_video = true;
  }
  if (fmt->audio_codec != AV_CODEC_ID_NONE) {
    add_stream(sequence_, &audio_st_, oc, &audio_codec, fmt->audio_codec);
    has_audio = encode_audio = true;
  }

  if (has_video) {
    ret = openVideo(oc, video_codec, &video_st_, opt);
    if (ret < 0) return;
  }
  if (has_audio) {
    ret = openAudio(oc, audio_codec, &audio_st_, opt);
    if (ret < 0) return;
  }

  if (!(fmt->flags & AVFMT_NOFILE)) {
    ret = avio_open(&oc->pb, std::string(filename_.begin(), filename_.end()).c_str(), AVIO_FLAG_WRITE);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->error("Could not open {}", std::string(filename_.begin(), filename_.end()).c_str());
      return;
    }
  }

  ret = avformat_write_header(oc, &opt);
  if (ret < 0) {
    spdlog::get(LOGGER_DEFAULT)->error("Error occurred when opening output file");
    return;
  }

  int progress_update_count = 0;
  while (encode_video || encode_audio) {
    if (encode_video &&
        (!encode_audio ||
          av_compare_ts(video_st_.next_pts, video_st_.enc->time_base,
                        audio_st_.next_pts, audio_st_.enc->time_base) <= 0)) {
      encode_video = !writeVideoFrame(video_renderer_, sequence_, oc, &video_st_);
    }
    else {
      ret = writeAudioFrame(sequence_, audio_render_context_, oc, &audio_st_);
      if (ret == 1) encode_audio = false;
    }
    if (progress_update_count ++ >= 100) {
      qreal progress = audio_st_.next_pts / sequence_->sample_rate() * sequence_->base_time() / (qreal)sequence_->duration();
      progress_update_count = 0;
      setProgress(progress);
    }
  }

  av_write_trailer(oc);

  if (has_video)
    closeStream(oc, &video_st_);
  if (has_audio)
    closeStream(oc, &audio_st_);

  if (!(fmt->flags & AVFMT_NOFILE))
      avio_closep(&oc->pb);

  avformat_free_context(oc);
}

EncodingTask::EncodingTask(sptr<Sequence> sequence,
    std::wstring filename,
    sptr<video_renderer::ISurfaceProvider> surface_provider,
    QSurfaceFormat& surface_format) :
  sequence_(sequence), filename_(filename), surface_provider_(surface_provider),
  surface_format_(surface_format) {
  sequence->setDuration(600);
}





EncodingService::EncodingService(sptr<ITaskService> task_service) :
  task_service_(task_service) {
  surface_provider_ = std::make_shared<SurfaceProvider>();
  surface_format_.setMajorVersion(3);
  surface_format_.setMinorVersion(0);
  surface_format_.setAlphaBufferSize(8);
  surface_format_.setRedBufferSize(8);
  surface_format_.setBlueBufferSize(8);
  surface_format_.setGreenBufferSize(8);
  surface_format_.setDepthBufferSize(8);
  surface_format_.setSamples(0);
}

QSharedPointer<Task> EncodingService::encode(sptr<Sequence> sequence,  std::wstring filename) {
  spdlog::get(LOGGER_DEFAULT)->info("[EncodingService] encode sequence: {}", sequence->id());
  auto task = QSharedPointer<EncodingTask>
      (new EncodingTask(sequence, filename, surface_provider_, surface_format_));
  task_service_->queueTask(task, [](QSharedPointer<Task>){});
  return task;
}




GUIThreadSurfaceProvider::GUIThreadSurfaceProvider() {
}

QSurface* GUIThreadSurfaceProvider::createSurface(const QSurfaceFormat& format) {
  auto surface = new QOffscreenSurface();
  surface->setFormat(format);
  surface->create();
  return surface;
}
void GUIThreadSurfaceProvider::releaseSurface(QSurface* surface) {
  delete surface;
}


SurfaceProvider::SurfaceProvider() {
  QObject::connect(this, &SurfaceProvider::createSurfaceRequested, this,
    [this](int surface_id, const QSurfaceFormat& format) {
    QOffscreenSurface* surface = new QOffscreenSurface();
    surface->setFormat(format);
    surface->create();
    std::unique_lock<std::mutex> lk(m_);
    surfaces_[surface_id] = surface;
    cv_.notify_all();
  }, Qt::QueuedConnection);
  QObject::connect(this, &SurfaceProvider::releaseSurfaceRequested, this,
    [this](QSurface* surface) {
    delete surface;
  }, Qt::QueuedConnection);
}

QSurface* SurfaceProvider::createSurface(const QSurfaceFormat& format) {
  int surface_id = surface_id_++;
  std::unique_lock<std::mutex> lk(m_);
  emit createSurfaceRequested(surface_id, format);
  cv_.wait(lk, [this, surface_id]{
    return surfaces_.count(surface_id);
  });
  assert(surfaces_.count(surface_id));
  auto surface = surfaces_[surface_id];
  surfaces_.erase(surface_id);
  return surface;
}

void SurfaceProvider::releaseSurface(QSurface* surface) {
  emit releaseSurfaceRequested(surface);
}

}