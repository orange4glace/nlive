#include "browser/widgets/timeline/ghost_clip_view.h"

#include <QPainter>
#include <QDebug>

#include "browser/widgets/timeline/scrollview/sequencescrollview.h"

namespace nlive {

namespace timelinewidget {

GhostClipView::GhostClipView(SequenceScrollView* scroll_view, int start_time, int end_time) :
  scroll_view_(scroll_view),
  start_time_(start_time),
  end_time_(end_time),
  extended_start_time_(start_time),
  extended_end_time_(end_time) {

}

void GhostClipView::updateView() {
  int left = scroll_view_->getPositionRelativeToView(extended_start_time_);
  int right = scroll_view_->getPositionRelativeToView(extended_end_time_);
  // TODO : FIXME
  // This fires ClipView to be repainted. but WHY?
  move(left, 0);
  resize(right - left, height());
}

void GhostClipView::resizeEvent(QResizeEvent* event) {
  updateView();
}

void GhostClipView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), QColor(35,125,90,125));
}

void GhostClipView::setExtendedTime(int start_time, int end_time) {
  extended_start_time_ = start_time;
  extended_end_time_ = end_time;
  updateView();
}

int GhostClipView::start_time() const {
  return start_time_;
}

int GhostClipView::end_time() const {
  return end_time_;
}

int GhostClipView::extended_start_time() const {
  return extended_start_time_;
}

int GhostClipView::extended_end_time() const {
  return extended_end_time_;
}

}

}