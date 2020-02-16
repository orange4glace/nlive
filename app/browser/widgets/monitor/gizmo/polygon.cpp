#include "browser/widgets/monitor/gizmo/polygon.h"

namespace nlive {

namespace monitor_widget {

AbstractPolygon::AbstractPolygon(QWidget* parent) :
  Div(parent) {
  
}

void AbstractPolygon::addPoint(int x, int y, int idx = -1) {
  if (idx == -1) idx = points_.size();
  Q_ASSERT(idx <= points_.size());
  Point* point = new Point(this);
  points_.insert(points_.begin() + idx, point);
}

Point* AbstractPolygon::getPointAt(int idx) {

}

}

}