#include "browser/widgets/timeline/scrollview/scrollbar.h"

#include "platform/theme/themeservice.h"

#include <QPainter>
#include <QMouseEvent>

#include <iostream>

namespace nlive {

/*#region HandleBar*/
SequenceScrollViewScrollbar::HandleBar::HandleBar(
  SequenceScrollViewScrollbar* scrollbar,
  QSharedPointer<IThemeService> theme_service) :
  QPushButton(scrollbar),
  scrollbar_(scrollbar),
  theme_service_(theme_service) {

}

void SequenceScrollViewScrollbar::HandleBar::mousePressEvent(QMouseEvent* event) {
  QPushButton::mousePressEvent(event);
  QPoint pos = mapToGlobal(event->pos());
  last_mouse_pos_ = pos;
}

void SequenceScrollViewScrollbar::HandleBar::mouseMoveEvent(QMouseEvent* event) {
  QPoint pos = mapToGlobal(event->pos());
  QPoint delta = pos - last_mouse_pos_;
  last_mouse_pos_ = pos;
  qreal rdx = delta.x() / (qreal)scrollbar_->width();
  qreal min = -scrollbar_->start();
  qreal max = 1 - scrollbar_->end();
  rdx = qMax(min, qMin(rdx, max));
  scrollbar_->setValue(scrollbar_->start() + rdx, scrollbar_->end() + rdx, true);
}

void SequenceScrollViewScrollbar::HandleBar::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  p.fillRect(QRectF(height() / 2.0f, 0, width() - height(), height()), theme.surfaceBrightColor());
}
/*#endreigon*/

/*#region HandleButton*/
SequenceScrollViewScrollbar::HandleButton::HandleButton(
  HandleBar* handlebar,
  SequenceScrollViewScrollbar* scrollbar,
  bool left,
  QSharedPointer<IThemeService> theme_service) :
  QPushButton(scrollbar),
  scrollbar_(scrollbar),
  left_(left),
  theme_service_(theme_service) {

}

void SequenceScrollViewScrollbar::HandleButton::mousePressEvent(QMouseEvent* event) {
  QPushButton::mousePressEvent(event);
  QPoint pos = mapToGlobal(event->pos());
  last_mouse_pos_ = pos;
}

void SequenceScrollViewScrollbar::HandleButton::mouseMoveEvent(QMouseEvent* event) {
  QPoint pos = mapToGlobal(event->pos());
  QPoint delta = pos - last_mouse_pos_;
  last_mouse_pos_ = pos;
  qreal rdx = delta.x() / (qreal)scrollbar_->width();
  if (left_) scrollbar_->setStart(scrollbar_->start() + rdx, true);
  else scrollbar_->setEnd(scrollbar_->end() + rdx, true);
}

void SequenceScrollViewScrollbar::HandleButton::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  p.setRenderHint(QPainter::RenderHint::Antialiasing);

  p.setPen(QPen(Qt::transparent));
  p.setBrush(QBrush(theme.surfaceContrastColor()));
  p.drawEllipse(QRect(0, 0, width(), height()));
  p.setBrush(QBrush(theme.surfaceBrightColor()));
  p.drawEllipse(QRectF(width() / 4.0f, height() / 4.0f, width() / 2.0f, height() / 2.0f));
}
/*#endreigon*/

SequenceScrollViewScrollbar::SequenceScrollViewScrollbar(
  QWidget* parent,
  qreal start,
  qreal end,
  QSharedPointer<IThemeService> theme_service) :
  QWidget(parent),
  theme_service_(theme_service),
  bar_handle_(this, theme_service),
  left_handle_(&bar_handle_, this, true, theme_service), right_handle_(&bar_handle_, this, false, theme_service),
  start_(start), end_(end) {

}

void SequenceScrollViewScrollbar::setStart(qreal value, bool from_internal) {
  start_ = qMax(0.0, qMin(value, end_));
  doUpdate(from_internal);
}

void SequenceScrollViewScrollbar::setEnd(qreal value, bool from_internal) {
  end_ = qMin(1.0, qMax(value, start_));
  doUpdate(from_internal);
}

void SequenceScrollViewScrollbar::setValue(qreal start, qreal end, bool from_internal) {
  start_ = qMax(0.0, start);
  end_ = qMin(1.0, end);
  if (start_ > end_) end_ = start_;
  doUpdate(from_internal);
}

void SequenceScrollViewScrollbar::doUpdate(bool from_internal) {
  int button_size = height();
  int left = start_ * width();
  int right = end_ * width();
  bar_handle_.move(left, bar_handle_.y());
  bar_handle_.resize(right - left, height());

  left_handle_.move(left, 0);
  left_handle_.resize(button_size, button_size);

  right_handle_.move(right - button_size, 0);
  right_handle_.resize(button_size, button_size);

  update();

  if (from_internal) emit onDidUpdateByUI();
  emit onDidUpdate();
}

void SequenceScrollViewScrollbar::resizeEvent(QResizeEvent* event) {
  doUpdate(true);
  QWidget::resizeEvent(event);
}

void SequenceScrollViewScrollbar::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
}

qreal SequenceScrollViewScrollbar::start() const {
  return start_;
}

qreal SequenceScrollViewScrollbar::end() const {
  return end_;
}


}