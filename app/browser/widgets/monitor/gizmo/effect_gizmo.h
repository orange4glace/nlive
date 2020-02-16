#ifndef NLIVE_MONITOR_WIDGET_EFFECT_GIZMO_H
#define NLIVE_MONITOR_WIDGET_EFFECT_GIZMO_H

#include "base/common/include.h"
#include "model/include.h"
#include "browser/widgets/monitor/gizmo/gizmo.h"

namespace nlive {

namespace monitor_widget {

class EffectGizmo : public Gizmo {

protected:
  sptr<Sequence> sequence_;
  sptr<Clip> clip_;
  sptr<effect::Effect> effect_;

  EffectGizmo(sptr<Sequence> sequence, sptr<Clip> clip,
    sptr<effect::Effect> effect);

  virtual void doUpdate() = 0;
  
};

}

}

#endif