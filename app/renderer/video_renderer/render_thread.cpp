#include "renderer/video_renderer/render_thread.h"

#include <chrono>
#include <QDebug>
#include <QOpenGLFunctions>

namespace nlive {

namespace video_renderer {

RenderThread::RenderThread(
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

RenderThread::~RenderThread() {
  qDebug() << "[VideoRenderer] Being destroy.";
  // TODO
  delete surface_;
  renderer_ctx_ = nullptr;
  sharing_ctx_ = nullptr;
  delete gl_;
  qDebug() << "[VideoRenderer] Renderer destroied";
}

void RenderThread::run() {
  qDebug() << "[Renderer] Thread started " << currentThreadId() << "\n";
  gl_->makeCurrent(surface_);
  buffer_swap_mutex_.lock();
  renderer_ctx_->initialize();
  buffer_swap_mutex_.unlock();
  auto gf = gl_->functions();

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

    gl_->makeCurrent(surface_);
    auto back_rt = renderer_ctx_->getBackRenderTexture();
    renderer_.render(renderer_ctx_, back_rt, command_buffer);
    gl_->doneCurrent();
  
    buffer_swap_mutex_.lock();
    renderer_ctx_->swapRenderTextures();
    buffer_swap_mutex_.unlock();
    
    emit onDidReadyData();
  }
  qDebug() << "[VideoRenderer] Thread ended.";
}

void RenderThread::render(sptr<CommandBuffer> command_buffer) {
  command_buffer_mutex_.lock();
  if (closed_) {
    command_buffer_mutex_.unlock();
    return;
  }
  current_command_buffer_ = command_buffer;
  command_buffer_wait_.notify_all();
  command_buffer_mutex_.unlock();
}

void RenderThread::close() {
  command_buffer_mutex_.lock();
  closed_ = true;
  qDebug() << "[Renderer] close!";
  command_buffer_wait_.notify_all();
  command_buffer_mutex_.unlock();
}

}

}