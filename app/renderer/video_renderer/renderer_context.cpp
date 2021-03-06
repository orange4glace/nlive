#include "renderer/video_renderer/renderer_context.h"

#include <QOpenGLFunctions>
#include <QDebug>

namespace nlive {

namespace video_renderer {

namespace {
  int next_render_texture_id = 0;
}

RendererContext::RendererContext(QOpenGLContext* gl,
    int width, int height,
    sptr<RenderSharingContext> sharing_context) :
  gl_(gl), width_(width), height_(height), sharing_context_(sharing_context),
  front_buffer_index_(0), initialized_(false) {
  decoder_manager_ = sptr<DecoderManager>(new DecoderManager());
}

RendererContext::~RendererContext() {
  for (auto& kv : sws_map_) {
    sws_freeContext(kv.second.context);
    delete kv.second.out_data;
  }
}

SwsValue* RendererContext::getSwsValue(SwsKey& key) {
  if (sws_map_.count(key)) return &sws_map_[key];
  SwsValue val;
  auto sws_ctx = sws_getContext(key.src_width, key.src_height, key.src_fmt,
      key.out_width, key.out_height, AV_PIX_FMT_RGB32, SWS_BILINEAR, NULL, NULL, NULL);
  uint8_t* out_data = new uint8_t[key.out_width * key.out_height * 4];
  val.context = sws_ctx;
  val.out_data = out_data;
  sws_map_[key] = val;
  return &sws_map_[key];
}
  
void RendererContext::initialize() { 
  if (initialized_) return;
  for (int i = 0; i < 2; i ++) {
    RenderTexture rt;
    rt.id = next_render_texture_id ++;
    rt.width = width_;
    rt.height = height_;
    auto glf = gl_->functions();
    glf->glGenFramebuffers(1, &rt.framebuffer);
    glf->glGenTextures(1, &rt.texture);
    glf->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rt.framebuffer);
    glf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    glf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glf->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.texture, 0);
    glf->glClearColor(0, 0, 0, 0);
    glf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glf->glBindTexture(GL_TEXTURE_2D, 0);
    glf->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    swap_buffers_.push_back(rt);
  }
  initialized_ = true;
}

RenderTexture RendererContext::createTemporaryRenderTexture(
  std::string name,
  int width,
  int height) {
  // TODO : Should assert?
  Q_ASSERT(!render_textures_.count(name));
  RenderTexture rt;
  rt.id = next_render_texture_id ++;
  rt.name = name;
  rt.width = width;
  rt.height = height;
  auto glf = gl_->functions();
  glf->glGenFramebuffers(1, &rt.framebuffer);
  glf->glGenTextures(1, &rt.texture);
  glf->glBindFramebuffer(GL_FRAMEBUFFER, rt.framebuffer);
  glf->glBindTexture(GL_TEXTURE_2D, rt.texture);
  glf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glf->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.texture, 0);
  glf->glBindTexture(GL_TEXTURE_2D, 0);
  glf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
  render_textures_.insert(make_pair(name, rt));
  return rt;
}

RenderTexture RendererContext::getTemporaryRenderTexture(std::string name) {
  Q_ASSERT(render_textures_.count(name));
  return render_textures_[name];
}

void RendererContext::releaseTemporaryRenderTexture(RenderTexture& rt) {
  releaseTemporaryRenderTexture(rt.name);
}

void RendererContext::releaseTemporaryRenderTexture(std::string name) {
  Q_ASSERT(render_textures_.count(name));
  RenderTexture rt = getTemporaryRenderTexture(name);
  auto glf = gl_->functions();
  glf->glDeleteFramebuffers(1, &rt.framebuffer);
  glf->glDeleteTextures(1, &rt.texture);
  render_textures_.erase(name);
}

void RendererContext::swapTemporaryRenderTexture(std::string lhs, std::string rhs) {
  Q_ASSERT(render_textures_.count(lhs) && render_textures_.count(rhs));
  auto tmp = render_textures_[lhs];
  render_textures_[lhs] = render_textures_[rhs];
  render_textures_[rhs] = tmp;
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

int RendererContext::width() const {
  return width_;
}

int RendererContext::height() const {
  return height_;
}

sptr<RenderSharingContext> RendererContext::sharing_context() {
  return sharing_context_;
}

sptr<DecoderManager> RendererContext::decoder_manager() {
  return decoder_manager_;
}

bool RendererContext::initialized() const {
  return initialized_;
}

QOpenGLContext* RendererContext::gl() {
  return gl_;
}

}

}