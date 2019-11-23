#ifndef NLIVE_MONITOR_WIDET_SEQUENCE_RENDERER_VIEW_H_
#define NLIVE_MONITOR_WIDET_SEQUENCE_RENDERER_VIEW_H_

#include "base/common/memory.h"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QThread>

#include "browser/services/play/play_service.h"
#include "renderer/video_renderer/sequence_renderer.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

class Sequence;

namespace monitor_widget {

class SequenceRendererView : public QOpenGLWidget, protected QOpenGLFunctions {

private:
  video_renderer::SequenceRenderer* renderer_;
  sptr<PlayService> play_service_;

  timeline_widget::SequenceView* timeline_widget_sequence_view_;
  sptr<Sequence> sequence_;

  void scheduleRender();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  SequenceRendererView(QWidget* parent, timeline_widget::SequenceView* timeline_widget_sequence_view,
    sptr<PlayService> play_service);
  ~SequenceRendererView();

};

}

}

#endif