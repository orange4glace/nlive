#include "browser/widgets/monitor/sequence_view.h"

#include <QOpenGLContext>
#include <QPainter>
#include <QRandomGenerator>

namespace nlive {

namespace monitor_widget {

SequenceView::SequenceView(QWidget* parent, timeline_widget::SequenceView* timeline_widget_sequence_view,
  sptr<PlayService> play_service) :
  QWidget(parent), play_service_(play_service), timeline_widget_sequence_view_(timeline_widget_sequence_view) {
  sequence_ = timeline_widget_sequence_view->sequence();
  renderer_view_ = new SequenceRendererView(this, timeline_widget_sequence_view, play_service_);
  renderer_view_->show();
}

void SequenceView::resizeEvent(QResizeEvent* e) {
  int ww = width();
  int hh = height();
  int sw = sequence_->width();
  int sh = sequence_->height();
  if (sw > ww) {
    float r = (float)ww / sw;
    sw *= r;
    sh *= r;
  }
  if (sh > hh) {
    float r = (float)hh / sh;
    sw *= r;
    sh *= r;
  }
  renderer_view_->resize(sw, sh);
  renderer_view_->move((ww - sw) / 2, (hh - sh) / 2);
}

void SequenceView::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  // p.fillRect(rect(), Qt::darkCyan);
}

}

}