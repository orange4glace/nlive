#ifndef _NLIVE_RENDER_COMMAND_H_
#define _NLIVE_RENDER_COMMAND_H_

#include <QOpenGLContext>
#include "renderer/video_renderer/renderer_context.h"

namespace nlive {

namespace video_renderer {

class RenderCommand {

public:
  virtual void render(QSharedPointer<RendererContext> ctx) = 0;

};

}

}

#endif