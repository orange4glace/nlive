#include "renderer/video_renderer/render_sync.h"

#include <chrono>
#include <QDebug>
#include <QOpenGLFunctions>

namespace nlive {

namespace video_renderer {

RenderSync::RenderSync(
  sptr<ISurfaceProvider> surface_provider,
  QSurfaceFormat& surface_format,
  QOpenGLContext* target_gl,
  int width, int height) :
  target_gl_(target_gl), width_(width), height_(height),
  surface_provider_(surface_provider) {
  qDebug() << "[RenderSync]" << surface_format;
  surface_ = surface_provider_->createSurface(surface_format);

  gl_ = new QOpenGLContext();
  gl_->create();
  gl_->makeCurrent(surface_);
  gl_->functions()->initializeOpenGLFunctions();

  sharing_ctx_ = sptr<RenderSharingContext>(
    new RenderSharingContext(gl_));
  sharing_ctx_->initialize();
  renderer_ctx_ = sptr<RendererContext>(
    new RendererContext(gl_, width, height, sharing_ctx_));

  renderer_ctx_->initialize();
  auto gf = gl_->functions();
  gl_->doneCurrent();
}

RenderSync::~RenderSync() {
  qDebug() << "[VideoRenderer] Being destroy.";
  // TODO
  surface_provider_->releaseSurface(surface_);
  renderer_ctx_ = nullptr;
  sharing_ctx_ = nullptr;
  delete gl_;
  qDebug() << "[VideoRenderer] Renderer destroied";
}

void RenderSync::readPixels(GLint x, GLint y, GLsizei width, GLsizei height,
    GLenum format, GLenum type, uint8_t* data) {
  gl_->makeCurrent(surface_);
  auto gf = gl_->functions();
  gf->glBindFramebuffer(GL_FRAMEBUFFER, renderer_ctx_->getFrontRenderTexture().framebuffer);
  gf->glReadPixels(x, y, width, height, format, type, data);
  gf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
  gl_->doneCurrent();
}

void RenderSync::render(sptr<CommandBuffer> command_buffer) {
  gl_->makeCurrent(surface_);
  auto back_rt = renderer_ctx_->getBackRenderTexture();
  renderer_.render(renderer_ctx_, back_rt, command_buffer);
  renderer_ctx_->swapRenderTextures();
  gl_->doneCurrent();
}

}

}