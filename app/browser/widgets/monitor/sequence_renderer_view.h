#ifndef NLIVE_MONITOR_WIDET_SEQUENCE_RENDERER_VIEW_H_
#define NLIVE_MONITOR_WIDET_SEQUENCE_RENDERER_VIEW_H_

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

class SequenceRendererView : public QOpenGLWidget, protected QOpenGLFunctions {

private:
  QSharedPointer<Sequence> sequence_;
  video_renderer::SequenceRenderer* sequence_renderer_;

  void scheduleRender();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  SequenceRendererView(QWidget* parent, QSharedPointer<Sequence> sequence);
  ~SequenceRendererView();

};

}

}

#endif