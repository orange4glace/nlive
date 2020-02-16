#include "browser/widgets/monitor/gizmo_view.h"

#include "platform/theme/themeservice.h"
#include "browser/services/widgets/widgets_service.h"
#include "browser/widgets/effect_control/effect_control_widget.h"

namespace nlive {

namespace monitor_widget {

GizmoView::GizmoView(
    QWidget* parent,
    sptr<IThemeService> theme_service,
    sptr<IWidgetsService> widgets_service) :
  Div(parent), effect_control_widget_(nullptr),
  theme_service_(theme_service), widgets_service_(widgets_service) {
}

void GizmoView::updateTargetEffectWidget() {
  auto widget = widgets_service_->getWidget(effect_control::EffectControlWidget::ID);
  if (effect_control_widget_ == widget) return;
  
}

}

}