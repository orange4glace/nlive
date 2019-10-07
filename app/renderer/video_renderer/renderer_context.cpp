#include "renderer/video_renderer/renderer_context.h"

#include <QOpenGLFunctions>

namespace nlive {

namespace video_renderer {

namespace {
  int next_render_texture_id = 0;
}

RendererContext::RendererContext(QOpenGLContext* gl) :
  gl_(gl), front_buffer_index_(0) {
}
  
void RendererContext::initialize() {
  for (int i = 0; i < 2; i ++) {
    RenderTexture rt;
    rt.id = next_render_texture_id ++;
    auto glf = gl_->functions();
    glf->glGenFramebuffers(1, &rt.framebuffer);
    glf->glGenTextures(1, &rt.texture);
    glf->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rt.framebuffer);
    glf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    glf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glf->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.texture, 0);
    glf->glClearColor(0.0f, i ? 1.0f : 0.0f, 1.0f, 1.0f);
    glf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glf->glBindTexture(GL_TEXTURE_2D, 0);
    glf->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    swap_buffers_.push_back(rt);
  }
}

RenderTexture RendererContext::getTemporaryRenderTexture() {
  RenderTexture rt;
  rt.id = next_render_texture_id ++;
  auto glf = gl_->functions();
  glf->glGenFramebuffers(1, &rt.framebuffer);
  glf->glGenTextures(1, &rt.texture);
  glf->glBindFramebuffer(GL_FRAMEBUFFER, rt.framebuffer);
  glf->glBindTexture(GL_TEXTURE_2D, rt.texture);
  glf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glf->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.texture, 0);
  glf->glBindTexture(GL_TEXTURE_2D, 0);
  glf->glBindFramebuffer(GL_FRAMEBUFFER, 0);

  
  return rt;
}

void RendererContext::releaseTemporaryRenderTexture(RenderTexture& rt) {

}

RenderTexture RendererContext::getFrontRenderTexture() {
  return swap_buffers_[front_buffer_index_];
}

RenderTexture RendererContext::getBackRenderTexture() {
  return swap_buffers_[(front_buffer_index_ + 1) % 2];
}

// void RendererContext::blit(RenderTexture& rt) {
//   auto glf = gl_->functions();
//   glf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

// int RendererContext::getProgram(const char* name) {

// }

void RendererContext::swapRenderTextures() {
  front_buffer_index_ = (front_buffer_index_ + 1) % 2;
}

QOpenGLContext* RendererContext::gl() {
  return gl_;
}

}

}