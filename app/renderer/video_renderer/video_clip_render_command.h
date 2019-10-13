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
    ctx->releaseTemporaryRenderTexture("clip");
    ctx->releaseTemporaryRenderTexture("clip_temp");
  }

};

}

}

#endif