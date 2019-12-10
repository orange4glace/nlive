#ifndef NLIVE_VIDEO_RENDERER_RENDERER_H_
#define NLIVE_VIDEO_RENDERER_RENDERER_H_

#include <vector>
#include <memory>
#include <QThread>
#include "base/common/memory.h"
#include <QOpenGLContext>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QOffscreenSurface>

#include "renderer/video_renderer/render_sharing_context.h"
#include "renderer/video_renderer/renderer_context.h"
#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace video_renderer {

class Renderer {
public:
  Renderer() = default;

  void render(
    sptr<RendererContext> renderer_context,
    RenderTexture& render_texture,
    sptr<CommandBuffer> command_buffer);

};

}

}

#endif