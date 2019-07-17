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
  width_(width), height_(height),
  texture_pool_size_(texture_pool_size),
  ready_texture_index_(-1), working_texture_index_(0) {
  surface_.create();

  buffers_.resize(texture_pool_size);
  textures_.resize(texture_pool_size);
  // TODO : delete
  texture_mutexes_.resize(texture_pool_size);
  for (int i = 0; i < texture_pool_size; i ++) {
    buffers_[i] = -1;
    textures_[i] = -1;
    texture_mutexes_[i] = new QMutex();
  }
}

void Renderer::run() {
  qDebug() << "Run!\n";

  gl_ = QSharedPointer<QOpenGLContext>(new QOpenGLContext());
  gl_->setFormat(target_gl_->format());
  gl_->setShareContext(target_gl_);
  gl_->create();
  gl_->moveToThread(this);

  // TODO: locl
  generateTextures(width_, height_);

  while (true) {
    command_buffer_mutex_.lock();
    if (current_command_buffer_ == nullptr) {
      qDebug() << "Wait for commands..\n";
      command_buffer_wait_.wait(&command_buffer_mutex_);
    }
    gl_->makeCurrent(&surface_);
    QSharedPointer<CommandBuffer> command_buffer = current_command_buffer_;
    current_command_buffer_ = nullptr;
    command_buffer_mutex_.unlock();

    buffer_swap_mutex_.lock();
    QMutex* texture_lock = texture_mutexes_[working_texture_index_];
    texture_lock->lock();
    working_texture_index_ = (working_texture_index_ + 1) % texture_pool_size_;
    buffer_swap_mutex_.unlock();
    // Do render
    texture_lock->unlock();

    buffer_swap_mutex_.lock();
    ready_texture_index_ = working_texture_index_;
    buffer_swap_mutex_.unlock();

    qDebug() << "Emit!..\n";
    gl_->doneCurrent();
    emit onDidReadyData();
  }
}

void Renderer::generateTextures(int width, int height) {
  deleteTextures();
  qDebug() << "Generate textures..\n";
  gl_->makeCurrent(&surface_);
  for (int i = 0; i < texture_pool_size_; i ++) {
    auto glf = gl_->functions();
    glf->glGenFramebuffers(1, &buffers_[i]);
    glf->glGenTextures(1, &textures_[i]);
    glf->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffers_[i]);
    glf->glBindTexture(GL_TEXTURE_2D, textures_[i]);
    glf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glf->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures_[i], 0);
    glf->glBindTexture(GL_TEXTURE_2D, 0);
    glf->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }
  gl_->doneCurrent();
}

void Renderer::deleteTextures() {
  qDebug() << "Delete textures..\n";
  gl_->makeCurrent(&surface_);
  for (int i = 0; i < texture_pool_size_; i ++) {
    if (buffers_[i] != -1) {
      gl_->functions()->glDeleteFramebuffers(1, &buffers_[i]);
      gl_->functions()->glDeleteTextures(1, &textures_[i]);
      buffers_[i] = -1;
    }
  }
  gl_->doneCurrent();
}

void Renderer::render(QSharedPointer<CommandBuffer> command_buffer) {
  command_buffer_mutex_.lock();
  current_command_buffer_ = command_buffer;
  command_buffer_wait_.notify_all();
  command_buffer_mutex_.unlock();
}

std::unique_ptr<RenderTexture> Renderer::getRenderData() {
  QMutexLocker buffer_swap_lock(&buffer_swap_mutex_);
  if (ready_texture_index_ == -1) return nullptr;
  QMutex* texture_lock = texture_mutexes_[ready_texture_index_];
  std::unique_ptr<RenderTexture> texture = std::make_unique<RenderTexture>();
  texture->lock = QSharedPointer<QMutexLocker>(new QMutexLocker(texture_lock));
  texture->buffer = buffers_[ready_texture_index_];
  texture->texture = textures_[ready_texture_index_];
  return std::move(texture);
}

}

}