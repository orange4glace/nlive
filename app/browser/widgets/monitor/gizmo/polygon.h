#ifndef NLIVE_MONITOR_WIDGET_GIZMO_POLYGON_H
#define NLIVE_MONITOR_WIDGET_GIZMO_POLYGON_H

#include <vector>
#include "base/layout/div.h"
#include "browser/widgets/monitor/gizmo/point.h"

namespace nlive {

namespace monitor_widget {

class AbstractPolygon : public Div {

private:
  std::vector<Point*> points_;

protected:
  AbstractPolygon(QWidget* parent);

public:
  void addPoint(int x, int y, int idx = -1);
  Point* getPointAt(int idx);

  sig2_t<void (int idx, int dx, int dy)> onPointTranslate;
  sig2_t<void (int dx, int dy)> onTranslate;
  
};

class Rectangle : public AbstractPolygon {



};

}

}

#endif