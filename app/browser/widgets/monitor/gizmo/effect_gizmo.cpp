#include "browser/widgets/monitor/gizmo/effect_gizmo.h"

#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/effect.h"

namespace nlive {

namespace monitor_widget {

EffectGizmo::EffectGizmo(sptr<Sequence> sequence, sptr<Clip> clip,
  sptr<effect::Effect> effect) :
    sequence_(sequence), clip_(clip), effect_(effect) {
}

}

}