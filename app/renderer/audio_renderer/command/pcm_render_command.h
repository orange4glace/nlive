#ifndef NLIVE_AUDIO_RENDERER_PCM_RENDER_COMMAND_H_
#define NLIVE_AUDIO_RENDERER_PCM_RENDER_COMMAND_H_

#include "renderer/audio_renderer/render_command.h"
#include <cstring>

namespace nlive {

namespace audio_renderer {

class PCMRenderCommand : public RenderCommand {

private:
  char* buffer_;
  int offset_;
  int length_;

public:
  inline PCMRenderCommand(char* buffer, int offset, int length) :
    buffer_(buffer), offset_(offset), length_(length) {

  }

  inline void render(QSharedPointer<RenderContext> ctx) override {
    auto data = (uint8_t*)ctx->data();
    memcpy(data, buffer_ + offset_, length_);
  }

};

}

}

#endif