#ifndef NLIVE_PLATFORM_ENCODER_ENCODING_SERVICE_IMPL_H_
#define NLIVE_PLATFORM_ENCODER_ENCODING_SERVICE_IMPL_H_

#include <mutex>
#include <condition_variable>
#include "platform/encoder/encoding_service.h"
#include "platform/include.h"
#include "platform/task/task.h"
#include "renderer/video_renderer/render_sync.h"
#include "renderer/audio_renderer/renderer.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

namespace nlive {

namespace {

struct OutputStream {
  OutputStream();

  AVStream* st;
  AVCodecContext* enc;

  int64_t next_pts;
  int samples_count;

  AVFrame* frame;
  AVFrame* tmp_frame;

  SwsContext* sws_ctx;
  SwrContext* swr_ctx;
};

}

class EncodingTask : public Task {

private:
  sptr<Sequence> sequence_;
  std::wstring filename_;

  sptr<video_renderer::ISurfaceProvider> surface_provider_;
  QSurfaceFormat surface_format_;
  QOpenGLContext* gl_ctx_;
  video_renderer::RenderSync* video_renderer_;
  sptr<audio_renderer::RenderContext> audio_render_context_;

  /* audio output */
  OutputStream audio_st_;
  uint8_t** src_samples_data_;
  int src_sample_linesize_;
  int src_nb_samples_;
  /* video output */
  OutputStream video_st_;
  AVFrame* frame_;
  AVPicture src_picture_, dst_picture_;
  int frame_count_;

  int openAudio(AVFormatContext* oc, AVCodec* codec,
      OutputStream* ost, AVDictionary* opt_arg);
  int openVideo(AVFormatContext* oc, AVCodec* codec,
      OutputStream* ost, AVDictionary* opt_arg);

protected:
  void run() override;

public:
  EncodingTask(sptr<Sequence> sequence,
      std::wstring filename,
      sptr<video_renderer::ISurfaceProvider> surface_provider,
      QSurfaceFormat& surface_format);

};

class EncodingService : public IEncodingService {

private:
  sptr<ITaskService> task_service_;
  sptr<video_renderer::ISurfaceProvider> surface_provider_;
  QSurfaceFormat surface_format_;

public:
  EncodingService(sptr<ITaskService> task_service);
  QSharedPointer<Task> encode(sptr<Sequence> sequence, std::wstring filename) override;

};

class GUIThreadSurfaceProvider : public video_renderer::ISurfaceProvider {

public:
  GUIThreadSurfaceProvider();
  QSurface* createSurface(const QSurfaceFormat& format) override;
  void releaseSurface(QSurface* surface) override;

};

class SurfaceProvider : public QObject, public video_renderer::ISurfaceProvider {
  Q_OBJECT

private:
  std::mutex m_;
  std::condition_variable cv_;
  std::map<int, QSurface*> surfaces_;
  int surface_id_;

public:
  SurfaceProvider();
  QSurface* createSurface(const QSurfaceFormat& format) override;
  void releaseSurface(QSurface* surface) override;

signals:
  void createSurfaceRequested(int surface_id, const QSurfaceFormat& format);
  void releaseSurfaceRequested(QSurface* surface);

};

}

#endif