#ifndef NLIVE_AUDIO_RENDERER_RENDER_COMMAND_H_
#define NLIVE_AUDIO_RENDERER_RENDER_COMMAND_H_

#include "base/common/memory.h"
#include <QOpenGLContext>
#include "renderer/audio_renderer/render_context.h"

namespace nlive {

namespace audio_renderer {

class RenderCommand {

public:
  virtual void render(sptr<RenderContext> ctx) = 0;

};

}

}

#endif