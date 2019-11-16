#ifndef NLIVE_MONITOR_WIDET_SEQUENCE_RENDERER_VIEW_H_
#define NLIVE_MONITOR_WIDET_SEQUENCE_RENDERER_VIEW_H_

#include <QSharedPointer>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QThread>

#include "browser/services/play/play_service.h"
#include "browser/widgets/monitor/sequence_playable.h"

namespace nlive {

class Sequence;

namespace monitor_widget {

class SequenceRendererView : public QOpenGLWidget, protected QOpenGLFunctions {

private:
  QSharedPointer<PlayService> play_service_;

  QSharedPointer<Sequence> sequence_;
  SequencePlayable* player_;

  void scheduleRender();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  SequenceRendererView(QWidget* parent, QSharedPointer<Sequence> sequence,
    QSharedPointer<PlayService> play_service);
  ~SequenceRendererView();

};

}

}

#endif