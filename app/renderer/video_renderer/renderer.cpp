#include "renderer/video_renderer/renderer.h"

#include <QDebug>
#include <QOpenGLFunctions>

namespace nlive {

namespace video_renderer {

Renderer::Renderer(
  QOpenGLContext* target_gl, 
  int width, int height,
  size_t texture_pool_size) :
  target_gl_(target_gl), current_command_buffer_(nullptr),
  width_(width), height_(height) {
  surface_ = new QOffscreenSurface();
  surface_->create();

  gl_ = new QOpenGLContext();
  gl_->setShareContext(target_gl_);
  gl_->setFormat(target_gl_->format());
  gl_->create();
  gl_->functions()->initializeOpenGLFunctions();

  renderer_ctx_ = QSharedPointer<RendererContext>(new RendererContext(gl_));

  gl_->moveToThread(this);
}

Renderer::~Renderer() {
  delete surface_;
}

void Renderer::run() {
  gl_->makeCurrent(surface_);
  renderer_ctx_->initialize();
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
    if (current_command_buffer_ == nullptr) {
      qDebug() << "Wait for commands..\n";
      command_buffer_wait_.wait(&command_buffer_mutex_);
    }
    // Got a CommandBuffer that can be rendered into surface
    QSharedPointer<CommandBuffer> command_buffer = current_command_buffer_;
    current_command_buffer_ = nullptr;
    command_buffer_mutex_.unlock();

    gl_->makeCurrent(surface_);
    for (auto command : command_buffer->commands()) {
      qDebug() << "Command " << command << "\n";
      command->render(renderer_ctx_);
    }
    gl_->doneCurrent();
    
    buffer_swap_mutex_.lock();
    renderer_ctx_->swapRenderTextures();
    buffer_swap_mutex_.unlock();
    
    emit onDidReadyData();
  }
}

void Renderer::render(QSharedPointer<CommandBuffer> command_buffer) {
  command_buffer_mutex_.lock();
  current_command_buffer_ = command_buffer;
  command_buffer_wait_.notify_all();
  command_buffer_mutex_.unlock();
}

}

}