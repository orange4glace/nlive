#ifndef _NLIVE_VIDEO_RENDERER_RENDERER_H_
#define _NLIVE_VIDEO_RENDERER_RENDERER_H_

#include <vector>
#include <memory>
#include <QThread>
#include <QSharedPointer>
#include <QOpenGLContext>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QOffscreenSurface>

#include "renderer/video_renderer/render_sharing_context.h"
#include "renderer/video_renderer/renderer_context.h"
#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace video_renderer {

// struct RenderTexture {
//   QSharedPointer<QMutexLocker> lock;
//   GLuint buffer;
//   GLuint texture;

//   inline RenderTexture() {}
// };

class Renderer : public QThread {
  Q_OBJECT

private:
  int width_;
  int height_;

  QOpenGLContext* target_gl_;
  // QSharedPointer<QOpenGLContext> gl_;
  QOpenGLContext* gl_;
  QOffscreenSurface* surface_;
  QSharedPointer<RenderSharingContext> sharing_ctx_;
  QSharedPointer<RendererContext> renderer_ctx_;

  QSharedPointer<CommandBuffer> current_command_buffer_;

  QMutex command_buffer_mutex_;
  QWaitCondition command_buffer_wait_;

  QMutex buffer_swap_mutex_;
  
  GLuint vert_shader_;
  GLuint frag_shader_;
  GLuint program_;
  GLuint position_loc_;
  GLuint texCoord_loc_;
  GLuint image_loc_;
  GLuint position_buffer_;
  GLuint texCoord_buffer_;
  GLuint tex_;
  
  bool closed_;

protected:
  void run() override;

public:
  Renderer(
    QOpenGLContext* target_gl,
    int width, int height,
    size_t texture_pool_size = 2);
  ~Renderer();


  void render(QSharedPointer<CommandBuffer> command_buffer);
  // std::unique_ptr<RenderTexture> getRenderData();
  void close();

  inline QSharedPointer<RendererContext> context() {
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