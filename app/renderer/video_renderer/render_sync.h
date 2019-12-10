#ifndef NLIVE_VIDEO_RENDERER_RENDER_SYNC_H_
#define NLIVE_VIDEO_RENDERER_RENDER_SYNC_H_

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
#include "renderer/video_renderer/surface_provider.h"

namespace nlive {

namespace video_renderer {

class RenderSync {

private:
  int width_;
  int height_;

  QOpenGLContext* target_gl_;
  QOpenGLContext* gl_;
  QSurface* surface_;
  Renderer renderer_;
  sptr<ISurfaceProvider> surface_provider_;
  sptr<RenderSharingContext> sharing_ctx_;
  sptr<RendererContext> renderer_ctx_;

public:
  RenderSync(
    sptr<ISurfaceProvider> surface_provider,
    QSurfaceFormat& surface_format,
    QOpenGLContext* target_gl,
    int width, int height);
  ~RenderSync();

  void readPixels(GLint x, GLint y, GLsizei width, GLsizei height,
      GLenum format, GLenum type, uint8_t* data);
  void render(sptr<CommandBuffer> command_buffer);

  inline sptr<RendererContext> context() {
    return renderer_ctx_;
  }

};

}

}

#endif