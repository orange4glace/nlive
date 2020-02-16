#ifndef NLIVE_MONITOR_WIDGET_TRANSFORM_GIZMO_H
#define NLIVE_MONITOR_WIDGET_TRANSFORM_GIZMO_H

#include "base/common/include.h"
#include "model/include.h"
#include "browser/widgets/monitor/gizmo/effect_gizmo.h"
#include "browser/widgets/monitor/gizmo/point.h"

namespace nlive {

namespace monitor_widget {

class TransformGizmo : public EffectGizmo {

protected:
  void doUpdate() override;

public:
  TransformGizmo(sptr<Sequence> sequence, sptr<Clip> clip,
    sptr<effect::Effect> effect);
  
};

}

}

#endif