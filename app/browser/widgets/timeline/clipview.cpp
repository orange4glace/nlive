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
  IThemeService* const theme_service) : 
  theme_service_(theme_service),
  QWidget(parent), track_(track), clip_(clip), focused_(false), scrollView_(scrollView),
  left_handle_inner_(this), left_handle_outer_(this),
  right_handle_inner_(this), right_handle_outer_(this) {

  updateView();
  QObject::connect(clip.get(), &Clip::onDidChangeTime, this, [this](int oldStart, int oldEnd) {
    updateView();
  });
  QObject::connect(scrollView, &SequenceScrollView::onDidUpdate, this, [this]() {
    updateView();
  });
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
  updateView();
  emit onDidFocus();
}

void ClipView::blur() {
  if (!focused_) return;
  focused_ = false;
  updateView();
  emit onDidBlur();
}

bool ClipView::focused() const {
  return focused_;
}

void ClipView::updateView() {
  const int HANDLE_MAX_SIZE = 15;
  const int BAR_HANDLE_MIN_SIZE = 15;
  QSharedPointer<Clip> next = nullptr;
  QSharedPointer<Clip> prev = nullptr;
  int next_pos = next ? scrollView_->getPositionRelativeToView(next->start_time()) : std::numeric_limits<int>::max();
  int prev_pos = prev ? scrollView_->getPositionRelativeToView(prev->end_time()) : std::numeric_limits<int>::min();
  int left = scrollView_->getPositionRelativeToView(clip_->start_time());
  int right = scrollView_->getPositionRelativeToView(clip_->end_time());
  int bar_size = right - left;
  int inner_handle_size = qMin(HANDLE_MAX_SIZE, (bar_size - BAR_HANDLE_MIN_SIZE) / 2);
  int left_outer_handle_size = (prev_pos == std::numeric_limits<int>::min() ? HANDLE_MAX_SIZE : qMin(HANDLE_MAX_SIZE, left - prev_pos));
  int right_outer_handle_size = qMin(HANDLE_MAX_SIZE, next_pos - right);

  int container_size = left_outer_handle_size + right_outer_handle_size + bar_size;

  move(left - left_outer_handle_size, 0);
  resize(container_size, height());
  bar_handle_.move(left_outer_handle_size, 0);
  bar_handle_.resize(right - left, height());
  left_handle_outer_.move(0, 0);
  right_handle_outer_.move(container_size - right_outer_handle_size, 0);
  left_handle_outer_.resize(left_outer_handle_size, height());
  right_handle_outer_.resize(right_outer_handle_size, height());
  if (inner_handle_size <= 0) {
    left_handle_inner_.hide();
    right_handle_inner_.hide();
  }
  else {
    left_handle_inner_.show();
    left_handle_inner_.move(left_outer_handle_size, 0);
    left_handle_inner_.resize(inner_handle_size, height());
    right_handle_inner_.show();
    right_handle_inner_.move(left_outer_handle_size + bar_size - inner_handle_size, 0);
    right_handle_inner_.resize(inner_handle_size, height());
  }
  repaint();
}

void ClipView::resizeEvent(QResizeEvent* event) {
  updateView();

  QWidget::resizeEvent(event);
}

void ClipView::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  p.fillRect(0, 0, width(), height(),
    focused_ ? theme.surfaceContrastColor() : QColor(216,119,119));
  p.fillRect(left_handle_outer_.x(), left_handle_outer_.y(), left_handle_outer_.width(), left_handle_outer_.height(),
    QColor(100,119,119,100));
  p.fillRect(left_handle_inner_.x(), left_handle_inner_.y(), left_handle_inner_.width(), left_handle_inner_.height() / 2,
    QColor(100,119,119,100));
  p.fillRect(right_handle_outer_.x(), right_handle_outer_.y(), right_handle_outer_.width(), right_handle_outer_.height(),
    QColor(100,119,119,100));
  QWidget::paintEvent(event);
}

ClipViewHandle ClipView::testHandle(QPoint&& point) const {
  qDebug() << geometry() << " " << point << "\n";
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