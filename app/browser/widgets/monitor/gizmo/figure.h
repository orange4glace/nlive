#ifndef NLIVE_MONITOR_WIDGET_GIZMO_FIGURE_H
#define NLIVE_MONITOR_WIDGET_GIZMO_FIGURE_H

#include "base/layout/div.h"
#include "base/common/include.h"
#include "browser/widgets/monitor/gizmo/gizmo_context.h"

namespace nlive {

namespace monitor_widget {

class Figure : public Div {

protected:
  sptr<GizmoContext> context_;

  virtual void doUpdate() = 0;

public:
  Figure(QWidget* parent,
      sptr<GizmoContext> context);
  
};

}

}

#endif