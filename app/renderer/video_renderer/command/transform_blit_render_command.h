#ifndef NLIVE_VIDEO_TRANFORM_BLIT_RENDER_COMMAND_H_
#define NLIVE_VIDEO_TRANFORM_BLIT_RENDER_COMMAND_H_

#include "base/common/memory.h"
#include <QOpenGLFunctions>
#include <iostream>
#include <stdint.h>

#include "base/common/perf.h"
#include "renderer/video_renderer/render_command.h"
#include "renderer/video_renderer/shader/solid_shader.h"
#include "renderer/video_renderer/shader/texture_shader.h"

namespace nlive {

namespace video_renderer {

class TransformBlitRenderCommand : public RenderCommand {

private:
  float x_, y_;

public:
  inline TransformBlitRenderCommand(float x, float y) :
    x_(x), y_(y) {

  }

  inline void render(sptr<RendererContext> ctx) {
    auto gf = ctx->gl()->functions();
    auto rt = ctx->getTemporaryRenderTexture("clip");
    auto back_rt = ctx->getBackRenderTexture();
    auto sharing_ctx = ctx->sharing_context();
    auto texture_shader = std::static_pointer_cast<TextureShaderProgram>(
        sharing_ctx->getShaderProgram("texture_shader"));

    gf->glBindFramebuffer(GL_FRAMEBUFFER, back_rt.framebuffer);
    gf->glViewport(0, 0, ctx->width(), ctx->height());
    gf->glUseProgram(texture_shader->id());
    gf->glActiveTexture(GL_TEXTURE0);
    gf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gf->glUniform1i(texture_shader->image(), 0);
    gf->glBindBuffer(GL_ARRAY_BUFFER, texture_shader->texCoord_buffer());
    float texCoord[12] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 1.0f,
      0.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 1.0f
    };
    gf->glBufferData(GL_ARRAY_BUFFER, 2 * 6 * 4, (void*)texCoord, GL_DYNAMIC_DRAW);
    gf->glEnableVertexAttribArray(texture_shader->texCoord());
    gf->glVertexAttribPointer(texture_shader->texCoord(), 2, GL_FLOAT, false, 0, (void*)0);
    gf->glBindBuffer(GL_ARRAY_BUFFER, texture_shader->position_buffer());
    float x1 = (x_ / ctx->width()) * 2 - 1;
    float y1 = (y_ / ctx->height()) * 2 - 1;
    float x2 = ((float)rt.width / ctx->width() + x_ / ctx->width()) * 2 - 1;
    float y2 = ((float)rt.height / ctx->height() + y_ / ctx->height()) * 2 - 1;
    // qDebug() << "Tblit " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
    float position[12] = {
      x1, y1,
      x2, y1,
      x1, y2,
      x1, y2,
      x2, y1,
      x2, y2
    };
    gf->glBufferData(GL_ARRAY_BUFFER, 2 * 6 * 4, (void*)position, GL_DYNAMIC_DRAW);
    gf->glEnableVertexAttribArray(texture_shader->position());
    gf->glVertexAttribPointer(texture_shader->position(), 2, GL_FLOAT, false, 0, (void*)0);
    gf->glDrawArrays(GL_TRIANGLES, 0, 6);

    gf->glDisableVertexAttribArray(texture_shader->texCoord_buffer());
    gf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    gf->glBindTexture(GL_TEXTURE_2D, 0);
    gf->glUseProgram(0);
  }

};

}

}

#endif