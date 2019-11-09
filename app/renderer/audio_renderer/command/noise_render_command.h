#ifndef NLIVE_AUDIO_RENDERER_NOISE_RENDER_COMMAND_H_
#define NLIVE_AUDIO_RENDERER_NOISE_RENDER_COMMAND_H_

#include "renderer/audio_renderer/render_command.h"

namespace nlive {

namespace audio_renderer {

class NoiseRenderCommand : public RenderCommand {

public:
  inline void render(QSharedPointer<RenderContext> ctx) override {
    auto data = (uint8_t*)ctx->data();
    for (int i = 0; i < ctx->buffer_size() / ctx->byte_per_sample(); i ++) {
      data[i] = i % 8;
    }
  }

};

}

}

#endif