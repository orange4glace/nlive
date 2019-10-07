#ifndef _NLIVE_MONITOR_WIDET_SEQUENCE_VIEW_H_
#define _NLIVE_MONITOR_WIDET_SEQUENCE_VIEW_H_

#include <QSharedPointer>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QThread>

#include "renderer/video_renderer/sequence_renderer.h"

namespace nlive {

class Sequence;

namespace monitor_widget {

class SequenceView : public QOpenGLWidget, protected QOpenGLFunctions {

private:
  QSharedPointer<Sequence> sequence_;
  video_renderer::SequenceRenderer* sequence_renderer_;
  QOpenGLShaderProgram program;

  void scheduleRender();

  int vertexLocation;
  int matrixLocation;
  int colorLocation;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  SequenceView(QWidget* parent, QSharedPointer<Sequence> sequence);

};

}

}

#endif