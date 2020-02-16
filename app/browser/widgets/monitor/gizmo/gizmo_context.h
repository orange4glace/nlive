#ifndef NLIVE_MONITOR_WIDGET_GIZMO_CONTEXT_H
#define NLIVE_MONITOR_WIDGET_GIZMO_CONTEXT_H

namespace nlive {

namespace monitor_widget {

class GizmoContext {

public:
  QPoint mapFromGlobal(QPoint& p) const;
  int mapDistanceFromGlobal(int x) const;

};

}

}

#endif