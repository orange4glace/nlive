#ifndef NLIVE_BASE_LAYOUT_DIV_H_
#define NLIVE_BASE_LAYOUT_DIV_H_

#include "base/layout/div.h"

namespace nlive {

void Div::recalculateContentRect() {
  content_rect_ = QWidget::rect() - QMargins(
    paddings_[Div::Direction::LEFT] + border_sizes_[1],
    paddings_[Div::Direction::TOP] + border_sizes_[0],
    paddings_[Div::Direction::RIGHT] + border_sizes_[2],
    paddings_[Div::Direction::BOTTOM] + border_sizes_[3]);
  contentRectUpdated();
}

void Div::resizeEvent(QResizeEvent* e) {
  recalculateContentRect();
  contentRectUpdated();
}

void Div::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  if (background_color_ != Qt::transparent) {
    p.fillRect(QWidget::rect(), background_color_);
  }
  int XR[4] = {0, -1, 0, 0};
  int YR[4] = {0, 0, -1, 0};
  int XW[4] = {0, width(), 0, 0};
  int YW[4] = {0, 0, height(), 0};
  int WR[4] = {0, 1, 0, 1};
  int HR[4] = {1, 0, 1, 1};
  int WW[4] = {width(), 0, width(), 0};
  int HH[4] = {0, height(), 0, height()};
  for (int i = 0; i < 4; i ++) {
    if (border_sizes_[i] > 0) {
      p.fillRect(
        XW[i] + XR[i] * border_sizes_[i],
        YW[i] + YR[i] * border_sizes_[i],
        WW[i] + WR[i] * border_sizes_[i],
        HH[i] + HR[i] * border_sizes_[i],
        border_colors_[i]);
    }
  }
}

void Div::contentRectUpdated() {}

Div::Div(QWidget* parent) :
  QWidget(parent),
  flex_(1), flex_grow_(1), flex_shrink_(1), flex_basis_(-1),
  background_color_(Qt::transparent) {
  for (int i = 0; i < 4; i ++) {
    paddings_[i] = 0;
    margins_[i] = 0;
    border_sizes_[i] = 0;
  }
  recalculateContentRect();
}

Div* Div::setFlex(float value) {
  flex_ = value;
  onFlexUpdate();
  updateGeometry();
  return this;
}
Div* Div::setFlexGrow(float value) {
  flex_grow_ = value;
  onFlexUpdate();
  updateGeometry();
  return this;
}
Div* Div::setFlexShrink(float value) {
  flex_shrink_ = value;
  onFlexUpdate();
  updateGeometry();
  return this;
}
Div* Div::setFlexBasis(int value) {
  flex_basis_ = value;
  onFlexUpdate();
  updateGeometry();
  return this;
}
Div* Div::setBackgroundColor(QColor color) {
  background_color_ = color;
  update();
  return this;
}
Div* Div::setPadding(Div::Direction direction, int size) {
  if (direction == Div::Direction::ALL)
    for (int i = 0; i < 4; i ++) paddings_[i] = size;
  else {
    paddings_[direction] = size;
  }
  recalculateContentRect();
  updateGeometry();
  return this;
}
Div* Div::setBorder(Div::Direction direction, int size, QColor color) {
  if (direction == Div::Direction::ALL)
    for (int i = 0; i < 4; i ++) {
      border_sizes_[i] = size;
      border_colors_[i] = color;
    }
  else {
    border_sizes_[direction] = size;
    border_colors_[direction] = color;
  }
  recalculateContentRect();
  update();
  return this;
}
void Div::setChildGeometry(QWidget* child, const QRect& rect) {
  child->setGeometry(
    rect.translated(content_rect_.x(), content_rect_.y()));
}
void Div::setChildGeometry(QWidget* child, int x, int y, int w, int h) {
  child->setGeometry(
    QRect(x, y, w, h).translated(content_rect_.x(), content_rect_.y()));
}

const QRect& Div::content_rect() {
  return content_rect_;
}

const QRect& Div::contentRect() {
  return content_rect_;
}

QRect Div::calcRelativeRect(const QRect& rect) {
  return rect.translated(content_rect_.left(), content_rect_.top());
}

}

#endif