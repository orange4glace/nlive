#ifndef _NLIVE_VIDEO_CLIP_RENDER_COMMAND_H_
#define _NLIVE_VIDEO_CLIP_RENDER_COMMAND_H_

#include <QSharedPointer>
#include <QOpenGLFunctions>
#include <iostream>
#include <stdint.h>

#include "renderer/video_renderer/render_command.h"
#include "decoder/video_decoder.h"
#include "model/resource/video_resource.h"
#include "renderer/video_renderer/shader/solid_shader.h"
#include "renderer/video_renderer/shader/texture_shader.h"

namespace nlive {

namespace video_renderer {

class VideoClipPreRenderCommand : public RenderCommand {

private:
  QSharedPointer<VideoResource> resource_;
  QSharedPointer<VideoDecoder> decoder_;
  int64_t timestamp_;

public:
  inline VideoClipPreRenderCommand(
    QSharedPointer<VideoResource> resource,
    QSharedPointer<VideoDecoder> decoder,
    int64_t timestamp) :
    resource_(resource), decoder_(decoder), timestamp_(timestamp) {
  }

  inline void render(QSharedPointer<RendererContext> ctx) {
    int width = resource_->width();
    int height = resource_->height();
    auto rt = ctx->createTemporaryRenderTexture("clip", width, height);
    auto rtt = ctx->createTemporaryRenderTexture("clip_temp", width, height);
    QSharedPointer<VideoFrame> video_frame = decoder_->decode(timestamp_);
    if (!video_frame) return;
    uint8_t* buf = new uint8_t[width * height * 4];
    video_frame->scale((void*)buf);
    auto gf = ctx->gl()->functions();
    gf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    gf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
    gf->glBindTexture(GL_TEXTURE_2D, 0);
    delete buf;
  }

};

class VideoClipPostRenderCommand : public RenderCommand {

private:
  QSharedPointer<VideoResource> resource_;
  int64_t timestamp_;

public:
  inline VideoClipPostRenderCommand(
    QSharedPointer<VideoResource> resource) :
    resource_(resource) {
  }

  inline void render(QSharedPointer<RendererContext> ctx) {
    auto gf = ctx->gl()->functions();
    auto rt = ctx->getTemporaryRenderTexture("clip");
    auto back_rt = ctx->getBackRenderTexture();
    auto sharing_ctx = ctx->sharing_context();
    auto texture_shader = qSharedPointerCast<TextureShaderProgram>(
        sharing_ctx->getShaderProgram("texture_shader"));

    gf->glBindFramebuffer(GL_FRAMEBUFFER, back_rt.framebuffer);
    gf->glViewport(0, 0, ctx->width(), ctx->height());
    gf->glClearColor(0, 0, 0, 1.0f);
    gf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    gf->glBufferData(GL_ARRAY_BUFFER, 4 * 12, (void*)texCoord, GL_DYNAMIC_DRAW);
    gf->glEnableVertexAttribArray(texture_shader->texCoord());
    gf->glVertexAttribPointer(texture_shader->texCoord(), 2, GL_FLOAT, false, 0, (void*)0);
    gf->glBindBuffer(GL_ARRAY_BUFFER, texture_shader->position_buffer());
    float x = (float)resource_->width() / ctx->width() * 2;
    float y = (float)resource_->height() / ctx->height() * 2;
    float position[12] = {
      -1.0f, -1.0f,
      -1.0f + x, -1.0f,
      -1.0f, -1.0f + y,
      -1.0f, -1.0f + y,
      -1.0f + x, -1.0f,
      -1.0f + x, -1.0f + y
    };
    gf->glBufferData(GL_ARRAY_BUFFER, 4 * 12, (void*)position, GL_DYNAMIC_DRAW);
    gf->glEnableVertexAttribArray(texture_shader->position());
    gf->glVertexAttribPointer(texture_shader->position(), 2, GL_FLOAT, false, 0, (void*)0);
    gf->glDrawArrays(GL_TRIANGLES, 0, 6);

    gf->glDisableVertexAttribArray(texture_shader->texCoord_buffer());
    gf->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    gf->glBindTexture(GL_TEXTURE_2D, 0);
    gf->glUseProgram(0);

    ctx->releaseTemporaryRenderTexture("clip");
    ctx->releaseTemporaryRenderTexture("clip_temp");
  }

};

}

}

#endif