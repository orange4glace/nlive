#ifndef NLIVE_DIV_H_
#define NLIVE_DIV_H_

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include "base/common/sig.h"

namespace nlive {

class Div : public QWidget, public Sig {

public:
  enum Direction {
    TOP = 0,
    RIGHT = 1,
    BOTTOM = 2,
    LEFT = 3,
    ALL = 4
  };

private:
  float flex_;
  float flex_grow_;
  float flex_shrink_;
  int flex_basis_;

  int paddings_[4];
  int margins_[4];
  int border_sizes_[4];
  QColor border_colors_[4];

  QRect content_rect_;

  inline void recalculateContentRect() {
    content_rect_ = QWidget::rect() - QMargins(
      paddings_[Div::Direction::LEFT] + border_sizes_[1],
      paddings_[Div::Direction::TOP] + border_sizes_[0],
      paddings_[Div::Direction::RIGHT] + border_sizes_[2],
      paddings_[Div::Direction::BOTTOM] + border_sizes_[3]);
    contentRectUpdated();
  }

protected:
  inline void resizeEvent(QResizeEvent* e) override {
    recalculateContentRect();
    contentRectUpdated();
  }

  inline void paintEvent(QPaintEvent* e) override {
    QPainter p(this);
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

  inline virtual void contentRectUpdated() {}

public:
  inline Div(QWidget* parent) :
    QWidget(parent),
    flex_(1), flex_grow_(1), flex_shrink_(1), flex_basis_(-1) {
    for (int i = 0; i < 4; i ++) {
      paddings_[i] = 0;
      margins_[i] = 0;
      border_sizes_[i] = 0;
    }
    recalculateContentRect();
  }

  inline Div* setFlex(float value) {
    flex_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline Div* setFlexGrow(float value) {
    flex_grow_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline Div* setFlexShrink(float value) {
    flex_shrink_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline Div* setFlexBasis(int value) {
    flex_basis_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline Div* setPadding(Div::Direction direction, int size) {
    if (direction == Div::Direction::ALL)
      for (int i = 0; i < 4; i ++) paddings_[i] = size;
    else {
      paddings_[direction] = size;
    }
    recalculateContentRect();
    updateGeometry();
    return this;
  }
  inline Div* setBorder(Div::Direction direction, int size, QColor color) {
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
  inline void setChildGeometry(QWidget* child, const QRect& rect) {
    child->setGeometry(
      rect.translated(content_rect_.x(), content_rect_.y()));
  }
  inline void setChildGeometry(QWidget* child, int x, int y, int w, int h) {
    child->setGeometry(
      QRect(x, y, w, h).translated(content_rect_.x(), content_rect_.y()));
  }

  inline const QRect& content_rect() {
    return content_rect_;
  }

  inline const QRect& contentRect() {
    return content_rect_;
  }

  inline QRect calcRelativeRect(const QRect& rect) {
    return rect.translated(content_rect_.left(), content_rect_.top());
  }

  inline float flex() const { return flex_; }
  inline float flex_grow() const { return flex_grow_; }
  inline float flex_shrink() const { return flex_shrink_; }
  inline int flex_basis() const { return flex_basis_; }

  inline int width() const { return content_rect_.width(); }
  inline int height() const { return content_rect_.height(); }
  inline const QRect& rect() const { return content_rect_; }

  sig2_t<void()> onFlexUpdate;

};

}

#endif