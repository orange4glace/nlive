#ifndef NLIVE_RENDER_COMMAND_H_
#define NLIVE_RENDER_COMMAND_H_

#include <QOpenGLContext>
#include "renderer/video_renderer/renderer_context.h"

namespace nlive {

namespace video_renderer {

class RenderCommand {

public:
  virtual void render(sptr<RendererContext> ctx) = 0;

};

}

}

#endif