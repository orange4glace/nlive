#ifndef NLIVE_MONITOR_WIDGET_GIZMO_POINT_H
#define NLIVE_MONITOR_WIDGET_GIZMO_POINT_H

#include "browser/widgets/monitor/gizmo/figure.h"

namespace nlive {

namespace monitor_widget {

class Point : public Figure {

private:
  int x_;
  int y_;

  int start_gx_;
  int start_gy_;
  int start_x_;
  int start_y_;

protected:
  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;

  void doUpdate() override;

public:
  Point(QWidget* parent,
      sptr<GizmoContext> context);

  void setX(int x);
  void setY(int y);
  void setPosition(int x, int y);
  int x() const;
  int y() const;

  sig2_t<void (int x, int y, int px, int py)> onMove;
  
};

}

}

#endif