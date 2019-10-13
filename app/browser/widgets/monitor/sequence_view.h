#ifndef _NLIVE_MONITOR_WIDET_SEQUENCE_VIEW_H_
#define _NLIVE_MONITOR_WIDET_SEQUENCE_VIEW_H_

#include <QSharedPointer>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QThread>

#include "browser/widgets/monitor/sequence_renderer_view.h"

namespace nlive {

class Sequence;

namespace monitor_widget {

class SequenceView : public QWidget {

private:
  QSharedPointer<Sequence> sequence_;
  SequenceRendererView* renderer_view_;

protected:
  void resizeEvent(QResizeEvent* e);
  void paintEvent(QPaintEvent* e);

public:
  SequenceView(QWidget* parent, QSharedPointer<Sequence> sequence);

};

}

}

#endif