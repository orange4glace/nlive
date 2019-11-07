#ifndef NLIVE_VIDEO_RENDERER_RENDERER_VIEW_H_
#define NLIVE_VIDEO_RENDERER_RENDERER_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <QOffscreenSurface>
#include <QOpenGLWidget>

#include "base/common/sig.h"
#include "renderer/video_renderer/renderer.h"
#include "model/sequence/sequence.h"

namespace nlive {

namespace video_renderer {

class CommandBuffer;

class RendererView : public QOpenGLWidget {
  Q_OBJECT

private:
  int width_, height_;
  bool initialized_;

  QSharedPointer<Renderer> renderer_;

  GLuint vert_shader_;
  GLuint frag_shader_;
  GLuint program_;
  GLuint position_loc_;
  GLuint texCoord_loc_;
  GLuint image_loc_;
  GLuint position_buffer_;
  GLuint texCoord_buffer_;
  GLuint tex_;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  RendererView(QWidget* parent, int width, int height);
  ~RendererView();

public slots:
  void render(QSharedPointer<CommandBuffer> command_buffer);

signals:
  void onDidReadyData();

};

}

}

#endif