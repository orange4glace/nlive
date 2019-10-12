#ifndef NLIVE_EFFECT_CONTROL_EFFECT_VIEW_PROXY_H_
#define NLIVE_EFFECT_CONTROL_EFFECT_VIEW_PROXY_H_

#include <QSharedPointer>
#include "base/common/sig.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/effect.h"
#include "platform/theme/themeservice-impl.h"

namespace nlive {

namespace effect_control {

class EffectViewProxy : public Sig {

private:
    QSharedPointer<Sequence> sequence_;
    QSharedPointer<Clip> clip_;
    QSharedPointer<effect::Effect> effect_;

public:
  EffectViewProxy(
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect);

};

}

}

#endif