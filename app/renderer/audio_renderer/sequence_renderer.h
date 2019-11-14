#ifndef NLIVE_AUDIO_RENDERER_SEQUENCE_RENDERER_H_
#define NLIVE_AUDIO_RENDERER_SEQUENCE_RENDERER_H_

#include <QObject>
#include <QSharedPointer>
#include "base/common/sig.h"
#include "model/sequence/sequence.h"

namespace nlive {

namespace audio_renderer {

class SequenceRenderer : public QObject, public Sig {

private:
  QSharedPointeR<Sequence> sequence_;

public:
  SequenceRenderer(
    QSharedPointer<Sequence> sequence);

};

}

}

#endif