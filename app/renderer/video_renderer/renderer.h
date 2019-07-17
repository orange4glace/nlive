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

#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace video_renderer {

struct RenderTexture {
  QSharedPointer<QMutexLocker> lock;
  GLuint buffer;
  GLuint texture;

  inline RenderTexture() {}
};

class Renderer : public QThread {
  Q_OBJECT

private:
  int width_;
  int height_;

  QOpenGLContext* target_gl_;
  QSharedPointer<QOpenGLContext> gl_;
  QOffscreenSurface surface_;
  QSharedPointer<CommandBuffer> current_command_buffer_;

  QMutex command_buffer_mutex_;
  QWaitCondition command_buffer_wait_;

  size_t texture_pool_size_;
  int ready_texture_index_;
  int working_texture_index_;
  QMutex buffer_swap_mutex_;
  std::vector<GLuint> buffers_;
  std::vector<GLuint> textures_;
  std::vector<QMutex*> texture_mutexes_;

  void generateTextures(int width, int height);
  void deleteTextures();

protected:
  void run() override;

public:
  Renderer(
    QOpenGLContext* target_gl,
    int width, int height,
    size_t texture_pool_size = 2);

  void render(QSharedPointer<CommandBuffer> command_buffer);
  std::unique_ptr<RenderTexture> getRenderData();

signals:
  void onDidReadyData();

};

}

}

#endif