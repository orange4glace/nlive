#ifndef NLIVE_VIDEO_RENDERER_RENDER_THREAD_H_
#define NLIVE_VIDEO_RENDERER_RENDER_THREAD_H_

#include <vector>
#include <memory>
#include <QThread>
#include <QOpenGLContext>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QOffscreenSurface>

#include "base/common/memory.h"
#include "renderer/video_renderer/render_sharing_context.h"
#include "renderer/video_renderer/renderer_context.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/video_renderer/renderer.h"

namespace nlive {

namespace video_renderer {

// struct RenderTexture {
//   sptr<QMutexLocker> lock;
//   GLuint buffer;
//   GLuint texture;

//   inline RenderTexture() {}
// };

class RenderThread : public QThread {
  Q_OBJECT

private:
  int width_;
  int height_;

  QOpenGLContext* target_gl_;
  // sptr<QOpenGLContext> gl_;
  QOpenGLContext* gl_;
  QOffscreenSurface* surface_;
  Renderer renderer_;
  sptr<RenderSharingContext> sharing_ctx_;
  sptr<RendererContext> renderer_ctx_;

  sptr<CommandBuffer> current_command_buffer_;

  QMutex command_buffer_mutex_;
  QWaitCondition command_buffer_wait_;

  QMutex buffer_swap_mutex_;
  
  bool closed_;

protected:
  void run() override;

public:
  RenderThread(
    QOpenGLContext* target_gl,
    int width, int height,
    size_t texture_pool_size = 2);
  ~RenderThread();


  void render(sptr<CommandBuffer> command_buffer);
  // std::unique_ptr<RenderTexture> getRenderData();
  void close();

  inline sptr<RendererContext> context() {
    return renderer_ctx_;
  }

  inline QMutex* buffer_swap_mutex() {
    return &buffer_swap_mutex_;
  }

signals:
  void onDidReadyData();

};

}

}

#endif