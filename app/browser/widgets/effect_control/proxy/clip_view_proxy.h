#ifndef NLIVE_EFFECT_CONTROL_CLIP_VIEW_PROXY_H_
#define NLIVE_EFFECT_CONTROL_CLIP_VIEW_PROXY_H_

#include <QSharedPointer>
#include "base/common/sig.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "platform/theme/themeservice-impl.h"

namespace nlive {

namespace effect_control {

class ClipViewProxy : public Sig {

private:
    QSharedPointer<Sequence> sequence_;
    QSharedPointer<Clip> clip_;

public:
  ClipViewProxy(
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip);

};

}

}

#endif