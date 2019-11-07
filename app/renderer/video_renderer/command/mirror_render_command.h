#ifndef NLIVE_VIDEO_MIRROR_RENDER_COMMAND_H_
#define NLIVE_VIDEO_MIRROR_RENDER_COMMAND_H_

#include <QSharedPointer>
#include <QOpenGLFunctions>
#include <iostream>
#include <stdint.h>

#include "renderer/video_renderer/render_command.h"
#include "renderer/video_renderer/shader/mirror_shader.h"

namespace nlive {

namespace video_renderer {

class MirrorRenderCommand : public RenderCommand {

private:

public:
  inline MirrorRenderCommand() {

  }

  inline void render(QSharedPointer<RendererContext> ctx) {
    auto gf = ctx->gl()->functions();
    auto rt = ctx->getTemporaryRenderTexture("clip");
    auto rtt = ctx->getTemporaryRenderTexture("clip_temp");
    auto back_rt = ctx->getBackRenderTexture();
    auto sharing_ctx = ctx->sharing_context();
    auto shader = qSharedPointerCast<MirrorShaderProgram>(
        sharing_ctx->getShaderProgram("mirror_shader"));

    gf->glBindFramebuffer(GL_FRAMEBUFFER, rtt.framebuffer);
    gf->glViewport(0, 0, rt.width, rt.height);
    gf->glUseProgram(shader->id());
    gf->glActiveTexture(GL_TEXTURE0);
    gf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gf->glUniform1i(shader->image(), 0);
    gf->glBindBuffer(GL_ARRAY_BUFFER, shader->texCoord_buffer());
    float texCoord[12] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 1.0f,
      0.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 1.0f
    };
    gf->glBufferData(GL_ARRAY_BUFFER, 4 * 12, (void*)texCoord, GL_DYNAMIC_DRAW);
    gf->glEnableVertexAttribArray(shader->texCoord());
    gf->glVertexAttribPointer(shader->texCoord(), 2, GL_FLOAT, false, 0, (void*)0);
    gf->glBindBuffer(GL_ARRAY_BUFFER, shader->position_buffer());
    float x1 = -1;
    float y1 = -1;
    float x2 = 1;
    float y2 = 1;
    float position[12] = {
      x1, y1,
      x2, y1,
      x1, y2,
      x1, y2,
      x2, y1,
      x2, y2
    };
    gf->glBufferData(GL_ARRAY_BUFFER, 4 * 12, (void*)position, GL_DYNAMIC_DRAW);
    gf->glEnableVertexAttribArray(shader->position());
    gf->glVertexAttribPointer(shader->position(), 2, GL_FLOAT, false, 0, (void*)0);
    gf->glDrawArrays(GL_TRIANGLES, 0, 6);

    gf->glDisableVertexAttribArray(shader->texCoord_buffer());
    gf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    gf->glBindTexture(GL_TEXTURE_2D, 0);
    gf->glUseProgram(0);

    ctx->swapTemporaryRenderTexture(rt.name, rtt.name);
  }

};

}

}

#endif