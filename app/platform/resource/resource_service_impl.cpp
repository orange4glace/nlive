#include "platform/resource/resource_service_impl.h"

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QDataStream>
#include <map>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
}

#include "platform/logger/logger.h"
#include "platform/task/task_service.h"
#include "platform/task/task.h"
#include "model/resource/video_resource.h"
#include "model/common/rational.h"

namespace nlive {

namespace {

struct VideoMetadata {
  AVRational time_base;
  AVRational frame_rate;
  int width;
  int height;
  int64_t duration;
};

struct AudioMetadata {
  AVRational time_base;
  int sample_rate;
  int64_t duration;
};

class AudioResourceRawConvertingTask : public Task {

private:
  QSharedPointer<AudioResource> resource_;

protected:
  void run() override {
    const std::string& path = resource_->path();
    spdlog::get(LOGGER_DEFAULT)->info("[AudioResourceRawConvertingTask] Start task. path = {}", path.c_str());

    AVCodec* dec = nullptr;
    AVCodecContext* dec_ctx = nullptr;
    AVStream* stream;
    int stream_index;
    AVDictionary* opts = nullptr;
    AVPacket* pkt;
    AVFrame* frame;

    QFile outfile(QString::fromStdString(path + ".raw"));
    outfile.open(QIODevice::WriteOnly);
    QDataStream outstream(&outfile);
    qDebug() << outstream.status();
    
    AVFormatContext* fmt_ctx = nullptr;
    int ret;
    ret = avformat_open_input(&fmt_ctx, resource_->path().c_str(), NULL, NULL);
    if (ret != 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avformat_open_input path = {} result = {}", path.c_str(), ret);
      return;
    }
    ret = avformat_find_stream_info(fmt_ctx, nullptr);
    if (ret != 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avformat_find_stream_info path = {} result = {}", path.c_str(), ret);
      avformat_close_input(&fmt_ctx);
      return;
    }
    stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (stream_index < 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to av_find_best_stream path = {} result = {}", path.c_str(), stream_index);
      avformat_close_input(&fmt_ctx);
      return;
    }
    stream = fmt_ctx->streams[stream_index];
    dec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (dec == nullptr) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_find_decoder path = {}", path.c_str());
      avformat_close_input(&fmt_ctx);
      return;
    }
    dec_ctx = avcodec_alloc_context3(dec);
    ret = avcodec_parameters_to_context(dec_ctx, stream->codecpar);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_parameters_to_context path = {} result = {}", path.c_str(), ret);
      avcodec_free_context(&dec_ctx);
      avformat_close_input(&fmt_ctx);
      return;
    }
    av_dict_set(&opts, "refcounted_frames", "0", 0);
    ret = avcodec_open2(dec_ctx, dec, &opts);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_open2 path = {} result = {}", path.c_str(), ret);
      avcodec_free_context(&dec_ctx);
      avformat_close_input(&fmt_ctx);
      return;
    }
    pkt = av_packet_alloc();
    frame = av_frame_alloc();
    spdlog::get(LOGGER_DEFAULT)->info("[AudioResourceRawConvertingTask] Audio sample format = {} sample rate = {} path = {}", dec_ctx->sample_fmt, dec_ctx->sample_rate, path.c_str());
    while (av_read_frame(fmt_ctx, pkt) >= 0) {
      if (pkt->stream_index == stream_index) {
        ret = avcodec_send_packet(dec_ctx, pkt);
        if (ret < 0) {
          spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_send_packet path = {} result = {}", path.c_str(), ret);
          avcodec_free_context(&dec_ctx);
          avformat_close_input(&fmt_ctx);
          av_packet_free(&pkt);
          av_frame_free(&frame);
          return;
        }
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret >= 0) {
          int byte_per_sample = av_get_bytes_per_sample((AVSampleFormat)frame->format);
          outstream.writeRawData((char*)frame->data[0], frame->nb_samples * byte_per_sample);
        }
        else if (ret == AVERROR(EAGAIN)) {
          continue;
        }
        else {
          spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_receive_frame path = {} result = {}", path.c_str(), ret);
          avcodec_free_context(&dec_ctx);
          avformat_close_input(&fmt_ctx);
          av_packet_free(&pkt);
          av_frame_free(&frame);
          return;
        }
      }
    }
    avcodec_free_context(&dec_ctx);
    avformat_close_input(&fmt_ctx);
    av_packet_free(&pkt);
    av_frame_free(&frame);
    spdlog::get(LOGGER_DEFAULT)->info("[AudioResourceRawConvertingTask] End task. path = {}", path.c_str());
  }

public:
  AudioResourceRawConvertingTask(QSharedPointer<AudioResource> resource) :
    resource_(resource) {

  }

};

// void getFirstVideoFrame(AVFormatContext* fmt_ctx, AVStream* stream) {
//   AVCodecContext* video_dec_ctx_ = nullptr;
//   if (open_codec_)
//   AVPacket pkt;
//   av_init_packet(&pkt);
//   pkt.data = nullptr;
//   pkt.size = 0;
//   int ret = 0;
//   int decoded = pkt.size;

//   if (pkt.)
// }

VideoMetadata* findBestVideoMetadata(QString path) {
  av_register_all();
  AVFormatContext* fmt_ctx = nullptr;
  AVDictionaryEntry* tag = nullptr;
  int ret;
  ret = avformat_open_input(&fmt_ctx, path.toStdString().c_str(), NULL, NULL);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_open_input path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }
  ret = avformat_find_stream_info(fmt_ctx, nullptr);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_find_stream_info path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }

  VideoMetadata* metadata = nullptr;
  for (int i = 0; i < fmt_ctx->nb_streams; i ++) {
    auto stream = fmt_ctx->streams[i];
    spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Search video stream. stream idx = {} codec_type = {}", i, stream->codecpar->codec_type);
    if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      metadata = new VideoMetadata();
      metadata->time_base = stream->time_base;
      metadata->frame_rate = stream->avg_frame_rate;
      metadata->width = stream->codec->width;
      metadata->height = stream->codec->height;
      metadata->duration = stream->duration;
      spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Found video stream. time_base = {}/{} avg_frame_rate = {}/{} duration = {}",
        stream->time_base.num, stream->time_base.den, stream->avg_frame_rate.num, stream->avg_frame_rate.den, stream->duration);
      avformat_free_context(fmt_ctx);
      return metadata;
    }
  }

  avformat_free_context(fmt_ctx);
  return nullptr;
}

AudioMetadata* findBestAudioMetadata(QString path) {
  av_register_all();
  AVFormatContext* fmt_ctx = nullptr;
  AVDictionaryEntry* tag = nullptr;
  int ret;
  ret = avformat_open_input(&fmt_ctx, path.toStdString().c_str(), NULL, NULL);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_open_input path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }
  ret = avformat_find_stream_info(fmt_ctx, nullptr);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_find_stream_info path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }

  AudioMetadata* metadata = nullptr;
  for (int i = 0; i < fmt_ctx->nb_streams; i ++) {
    auto stream = fmt_ctx->streams[i];
    spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Search audio stream. stream idx = {} codec_type = {}", i, stream->codecpar->codec_type);
    if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      metadata = new AudioMetadata();
      metadata->time_base = stream->time_base;
      metadata->sample_rate = stream->codec->sample_rate;
      metadata->duration = stream->duration;
      spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Found aidop stream. duration = {} sample_rate = {}",
          metadata->duration, metadata->sample_rate);
      avformat_free_context(fmt_ctx);
      return metadata;
    }
  }

  avformat_free_context(fmt_ctx);
  return nullptr;
}

}


AudioResourceRawConvertingService::AudioResourceRawConvertingService(QSharedPointer<ITaskService> task_service) :
  task_service_(task_service) {}

void AudioResourceRawConvertingService::process(QSharedPointer<AudioResource> resource) {
  auto task = new AudioResourceRawConvertingTask(resource);
  task_service_->queueTask(task, [](Task* task){});
}


ResourceService::ResourceService(QSharedPointer<ITaskService> task_service) :
  task_service_(task_service),
  audio_resource_raw_converting_service_(task_service) {
  
}

QSharedPointer<VideoResource> ResourceService::loadBestVideoResource(QString path) {
  VideoMetadata* metadata = findBestVideoMetadata(path);
  if (metadata) {
    QSharedPointer<VideoResource> resource = QSharedPointer<VideoResource>(
      new VideoResource(path.toStdString(),
          Rational::fromAVRational(metadata->time_base),
          Rational::fromAVRational(metadata->frame_rate),
          metadata->duration,
          metadata->width, metadata->height));
    delete metadata;
    return resource;
  }
  return nullptr;
}

QSharedPointer<AudioResource> ResourceService::loadBestAudioResource(QString path) {
  AudioMetadata* metadata = findBestAudioMetadata(path);
  if (metadata) {
    QSharedPointer<AudioResource> resource = QSharedPointer<AudioResource>(
      new AudioResource(path.toStdString(),
          Rational::fromAVRational(metadata->time_base),
          metadata->sample_rate,
          metadata->duration));
    audio_resource_raw_converting_service_.process(resource);
    delete metadata;
    return resource;
  }
  return nullptr;
}

}