#include "browser/widgets/timeline/clipview.h"

#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QCursor>
#include <QDebug>
#include <iostream>
#include <limits>

#include "model/sequence/clip.h"
#include "model/sequence/track.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "platform/theme/themeservice.h"

namespace nlive {

namespace timelinewidget {

ClipView::ClipView(
  QWidget* const parent,
  QSharedPointer<Track> track,
  QSharedPointer<Clip> clip,
  SequenceScrollView* const scrollView,
  QSharedPointer<IThemeService> const theme_service) : 
  theme_service_(theme_service),
  QWidget(parent), track_(track), clip_(clip), focused_(false), scrollView_(scrollView),
  left_handle_inner_(this), left_handle_outer_(this),
  right_handle_inner_(this), right_handle_outer_(this) {

  // updateView();
  // clip->onDidChangeTime.connect(sig2_t<void (int64_t, int64_t, int64_t)>::slot_type(
  //   [this](int64_t old_start_time, int64_t old_end_time, int64_t old_b_time) {
  //     updateView();
  //   }).track(__sig_scope_));
  // scrollView->onDidUpdate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
  //   boost::bind(&ClipView::updateView, this))));

  initializeHandles();
}

void ClipView::initializeHandles() {
  auto cursor_e = QCursor(QPixmap(":/timeline_widget/resize-e.png"));
  auto cursor_w = QCursor(QPixmap(":/timeline_widget/resize-w.png"));

  left_handle_inner_.setCursor(cursor_e);
  left_handle_outer_.setCursor(cursor_w);
  right_handle_outer_.setCursor(cursor_e);
  right_handle_inner_.setCursor(cursor_w);
}

void ClipView::focus() {
  if (focused_) return;
  focused_ = true;
  update();
  emit onDidFocus();
}

void ClipView::blur() {
  if (!focused_) return;
  focused_ = false;
  update();
  emit onDidBlur();
}

bool ClipView::focused() const {
  return focused_;
}

void ClipView::setLayout(int outer_left_handle_width, int inner_left_handle_width,
  int handle_width, int inner_right_handle_width, int outer_right_handle_width,
  bool border_left, bool border_right) {
  left_handle_outer_.resize(outer_left_handle_width, height());
  left_handle_inner_.resize(inner_left_handle_width, height());
  right_handle_outer_.resize(outer_right_handle_width, height());
  right_handle_inner_.resize(inner_right_handle_width, height());
  bar_handle_.resize(handle_width, height());
  border_left_ = border_left;
  border_right_ = border_right;
  updateGeometry();
}

void ClipView::resizeEvent(QResizeEvent* event) {
  left_handle_outer_.setGeometry(0, 0,
      left_handle_outer_.width(), height());
  left_handle_inner_.setGeometry(left_handle_outer_.width(), 0,
      left_handle_inner_.width(), height());
  right_handle_outer_.setGeometry(left_handle_outer_.width() + bar_handle_.width(), 0,
      right_handle_outer_.width(), height());
  right_handle_inner_.setGeometry(left_handle_outer_.width() + bar_handle_.width() - right_handle_inner_.width(), 0,
      right_handle_inner_.width(), height());
  bar_handle_.setGeometry(left_handle_outer_.width(), 0, bar_handle_.width(), height());
}

void ClipView::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  p.fillRect(bar_handle_.x(), bar_handle_.y(), bar_handle_.width(), height(),
    focused_ ? theme.surfaceContrastColor() : QColor(216,119,119));
  // p.fillRect(left_handle_outer_.x(), left_handle_outer_.y(), left_handle_outer_.width(), height(),
  //   QColor(100,119,119,100));
  // p.fillRect(left_handle_inner_.x(), left_handle_inner_.y(), left_handle_inner_.width(), height() / 2,
  //   QColor(100,119,119,100));
  // p.fillRect(right_handle_outer_.x(), right_handle_outer_.y(), right_handle_outer_.width(), height(),
  //   QColor(100,119,119,100));
  p.setPen(theme.surfaceColor());
  if (border_left_)
    p.drawLine(bar_handle_.geometry().x(), 0, bar_handle_.geometry().x(), height());
  if (border_right_)
    p.drawLine(bar_handle_.geometry().right(), 0, bar_handle_.geometry().right(), height());
}

ClipViewHandle ClipView::testHandle(QPoint&& point) const {
  if (!rect().contains(point)) return ClipViewHandle::NONE;
  if (left_handle_inner_.geometry().contains(point)) return ClipViewHandle::LEFT_INNER;
  if (left_handle_outer_.geometry().contains(point)) return ClipViewHandle::LEFT_OUTER;
  if (right_handle_inner_.geometry().contains(point)) return ClipViewHandle::RIGHT_INNER;
  if (right_handle_outer_.geometry().contains(point)) return ClipViewHandle::RIGHT_OUTER;
  if (bar_handle_.geometry().contains(point)) return ClipViewHandle::BAR;
  return ClipViewHandle::NONE;
}

QSharedPointer<Clip> ClipView::clip() {
  return clip_;
}

}

}