#ifndef NLIVE_AUDIO_RENDERER_NOISE_RENDER_COMMAND_H_
#define NLIVE_AUDIO_RENDERER_NOISE_RENDER_COMMAND_H_

#include "renderer/audio_renderer/render_command.h"

namespace nlive {

namespace audio_renderer {

class NoiseRenderCommand : public RenderCommand {

public:
  inline void render(sptr<RenderContext> ctx) override {
    auto data = (float*)ctx->data();
    int z = 0;
    for (int i = 0; i < ctx->nb_channels(); i ++) {
      for (int j = 0; j < ctx->samples_per_channel(); j ++) {
        data[z++] = z / (float)128;
      }
    }
  }

};

}

}

#endif