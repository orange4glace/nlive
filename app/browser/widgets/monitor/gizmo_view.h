#ifndef NLIVE_MONITOR_WIDGET_GIZMO_VIEW_H
#define NLIVE_MONITOR_WIDGET_GIZMO_VIEW_H

#include <QWidget>
#include "base/common/include.h"
#include "base/layout/div.h"
#include "platform/include.h"
#include "browser/services/include.h"
#include "browser/widgets/effect_control/effect_control_widget.h"

namespace nlive {

namespace monitor_widget {

class GizmoView : public Div {

private:
  sptr<IThemeService> theme_service_;
  sptr<IWidgetsService> widgets_service_;

  effect_control::EffectControlWidget* effect_control_widget_;

  void updateTargetEffectWidget();

public:
  GizmoView(
    QWidget* parent,
    sptr<IThemeService> theme_service,
    sptr<IWidgetsService> widgets_service);

};

}

}

#endif