#ifndef NLIVE_AUDIO_RENDERER_SEQUENCE_RENDERER_H_
#define NLIVE_AUDIO_RENDERER_SEQUENCE_RENDERER_H_

#include <QObject>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "model/sequence/sequence.h"
#include "renderer/audio_renderer/renderer.h"

namespace nlive {

namespace audio_renderer {

class SequenceRenderer : public QObject, public Sig {

private:
  sptr<Renderer> renderer_;
  sptr<Sequence> sequence_;

  int64_t base_frame_;
  bool playing_;

  void playingCallback(int index, int64_t start_frame, int64_t end_frame);

public:
  SequenceRenderer(
    sptr<Sequence> sequence);

  void start();
  void stop();
  void render();

};

}

}

#endif