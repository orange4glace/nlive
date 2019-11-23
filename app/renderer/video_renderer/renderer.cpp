#include "renderer/video_renderer/renderer.h"

#include <chrono>
#include <QDebug>
#include <QOpenGLFunctions>

namespace nlive {

namespace video_renderer {

Renderer::Renderer(
  QOpenGLContext* target_gl, 
  int width, int height,
  size_t texture_pool_size) :
  target_gl_(target_gl), current_command_buffer_(nullptr),
  width_(width), height_(height), closed_(false) {
  surface_ = new QOffscreenSurface();
  surface_->create();

  gl_ = new QOpenGLContext();
  gl_->setShareContext(target_gl_);
  gl_->setFormat(target_gl_->format());
  gl_->create();
  gl_->functions()->initializeOpenGLFunctions();

  sharing_ctx_ = sptr<RenderSharingContext>(
    new RenderSharingContext(gl_));
  sharing_ctx_->initialize();
  renderer_ctx_ = sptr<RendererContext>(
    new RendererContext(gl_, width, height, sharing_ctx_));

  gl_->moveToThread(this);
}

Renderer::~Renderer() {
  qDebug() << "[VideoRenderer] Being destroy.";
  // TODO
  delete surface_;
  renderer_ctx_ = nullptr;
  sharing_ctx_ = nullptr;
  delete gl_;
  qDebug() << "[VideoRenderer] Renderer destroied";
}

void Renderer::run() {
  qDebug() << "[Renderer] Thread started " << currentThreadId() << "\n";
  gl_->makeCurrent(surface_);
  buffer_swap_mutex_.lock();
  renderer_ctx_->initialize();
  buffer_swap_mutex_.unlock();
  auto gf = gl_->functions();

  vert_shader_ = gf->glCreateShader(GL_VERTEX_SHADER);
  frag_shader_ = gf->glCreateShader(GL_FRAGMENT_SHADER);
  
  const char* vert_shader_source_ =
      "#version 300 es\n"
      "in vec2 a_position;\n"
      "void main() {\n"
        "gl_Position = vec4(a_position, 0, 1);\n"
      "}";
  const char* frag_shader_source_ = 
      "#version 300 es\n"
      "precision mediump float;\n"
      "out vec4 outColor;\n"
      "void main() {\n"
        "outColor = vec4(1, 0, 0, 1);\n"
      "}";
  gf->glShaderSource(vert_shader_, 1, &vert_shader_source_, NULL);
  gf->glShaderSource(frag_shader_, 1, &frag_shader_source_, NULL);

  gf->glCompileShader(vert_shader_);
  gf->glCompileShader(frag_shader_);

  program_ = gf->glCreateProgram();
  gf->glAttachShader(program_, vert_shader_);
  gf->glAttachShader(program_, frag_shader_);
  gf->glLinkProgram(program_);

  position_loc_ = gf->glGetAttribLocation(program_, "a_position");

  gf->glGenBuffers(1, &position_buffer_);
  gl_->doneCurrent();

  while (true) {
    command_buffer_mutex_.lock();
    if (!closed_ && current_command_buffer_ == nullptr) {
      command_buffer_wait_.wait(&command_buffer_mutex_);
    }

    if (closed_) {
      command_buffer_mutex_.unlock();
      break;
    }

    // Got a CommandBuffer that can be rendered into surface
    sptr<CommandBuffer> command_buffer = current_command_buffer_;
    current_command_buffer_ = nullptr;
    command_buffer_mutex_.unlock();

// std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    gl_->makeCurrent(surface_);
    auto back_rt = renderer_ctx_->getBackRenderTexture();
    gf->glBindFramebuffer(GL_FRAMEBUFFER, back_rt.framebuffer);
    gf->glViewport(0, 0, back_rt.width, back_rt.height);
    gf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    for (auto command : command_buffer->commands()) {
      command->render(renderer_ctx_);
// std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
// qDebug() << "Command end = " <<  sec.count();  
    }
    gl_->doneCurrent();
// std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
// qDebug() << "Render = " <<  sec.count();  
  
    buffer_swap_mutex_.lock();
    renderer_ctx_->swapRenderTextures();
    buffer_swap_mutex_.unlock();
    
    emit onDidReadyData();
  }
  qDebug() << "[VideoRenderer] Thread ended.";
}

void Renderer::render(sptr<CommandBuffer> command_buffer) {
  command_buffer_mutex_.lock();
  if (closed_) {
    command_buffer_mutex_.unlock();
    return;
  }
  current_command_buffer_ = command_buffer;
  command_buffer_wait_.notify_all();
  command_buffer_mutex_.unlock();
}

void Renderer::close() {
  command_buffer_mutex_.lock();
  closed_ = true;
  qDebug() << "[Renderer] close!";
  command_buffer_wait_.notify_all();
  command_buffer_mutex_.unlock();
}

}

}