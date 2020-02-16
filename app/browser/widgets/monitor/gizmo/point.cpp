#include "browser/widgets/monitor/gizmo/point.h"

#include <QMouseEvent>

namespace nlive {

namespace monitor_widget {

Point::Point(QWidget* parent, sptr<GizmoContext> context) :
    Figure(parent, context) {

}

void Point::mousePressEvent(QMouseEvent* e) {
  start_gx_ = e->globalX();
  start_gy_ = e->globalY();
  start_x_ = x_;
  start_y_ = y_;
}

void Point::mouseMoveEvent(QMouseEvent* e) {
  int dgx = e->globalX() - start_gx_;
  int dgy = e->globalY() - start_gy_;
  start_gx_ = e->globalX();
  start_gy_ = e->globalY();
  int dx = context_->mapDistanceFromGlobal(dgx);
  int dy = context_->mapDistanceFromGlobal(dgy);
  int nx = start_x_ + dx;
  int ny = start_y_ + dy;
}

void Point::doUpdate() {
  QPoint p = context_->mapFromGlobal(QPoint(x_, y_));
  move(p);
}

void Point::setX(int x) {
  setPosition(x, y_);
}

void Point::setY(int y) {
  setPosition(x_, y);
}

void Point::setPosition(int x, int y) {
  x_ = x;
  y_ = y;
  doUpdate();
}

int Point::x() const {
  return x_;
}

int Point::y() const {
  return y_;
}

}

}