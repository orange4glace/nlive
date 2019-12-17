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
  QColor background_color_;

  QRect content_rect_;

  void recalculateContentRect();

protected:
  void resizeEvent(QResizeEvent* e) override;

  void paintEvent(QPaintEvent* e) override;

  virtual void contentRectUpdated();

public:
  Div(QWidget* parent);

  Div* setFlex(float value);
  Div* setFlexGrow(float value);
  Div* setFlexShrink(float value);
  Div* setFlexBasis(int value);
  Div* setBackgroundColor(QColor color);
  Div* setPadding(Div::Direction direction, int size);
  Div* setBorder(Div::Direction direction, int size, QColor color);
  void setChildGeometry(QWidget* child, const QRect& rect);
  void setChildGeometry(QWidget* child, int x, int y, int w, int h);

  const QRect& content_rect();
  const QRect& contentRect();

  QRect calcRelativeRect(const QRect& rect);

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