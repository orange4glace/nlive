#include "renderer/video_renderer/renderer.h"

#include <chrono>
#include <QDebug>
#include <QOpenGLFunctions>

namespace nlive {

namespace video_renderer {

void Renderer::render(
    sptr<RendererContext> renderer_context,
    RenderTexture& render_texture,
    sptr<CommandBuffer> command_buffer) {
// std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    auto gf = renderer_context->gl()->functions();
    gf->glBindFramebuffer(GL_FRAMEBUFFER, render_texture.framebuffer);
    gf->glViewport(0, 0, render_texture.width, render_texture.height);
    gf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    for (auto command : command_buffer->commands()) {
      command->render(renderer_context);
// std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
// qDebug() << "Command end = " <<  sec.count();  
    }
// std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
// qDebug() << "Render = " <<  sec.count();  
  }
}

}