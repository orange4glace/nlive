#ifndef NLIVE_MONITOR_WIDET_SEQUENCE_VIEW_H_
#define NLIVE_MONITOR_WIDET_SEQUENCE_VIEW_H_

#include <QSharedPointer>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QThread>

#include "browser/widgets/monitor/sequence_renderer_view.h"
#include "browser/services/play/play_service.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

class Sequence;

namespace monitor_widget {

class SequenceView : public QWidget {

private:
  QSharedPointer<PlayService> play_service_;

  timeline_widget::SequenceView* timeline_widget_sequence_view_;
  QSharedPointer<Sequence> sequence_;
  SequenceRendererView* renderer_view_;

protected:
  void resizeEvent(QResizeEvent* e);
  void paintEvent(QPaintEvent* e);

public:
  SequenceView(QWidget* parent, timeline_widget::SequenceView* timeline_widget_sequence_view,
    QSharedPointer<PlayService> play_service);

};

}

}

#endif