#ifndef NLIVE_VIDEO_CLIP_RENDER_COMMAND_H_
#define NLIVE_VIDEO_CLIP_RENDER_COMMAND_H_

#include "base/common/memory.h"
#include <QOpenGLFunctions>
#include <iostream>
#include <stdint.h>

#include "base/common/perf.h"
#include "renderer/video_renderer/render_command.h"
#include "decoder/video_decoder.h"
#include "model/resource/video_resource.h"
#include "renderer/video_renderer/shader/solid_shader.h"
#include "renderer/video_renderer/shader/texture_shader.h"

namespace nlive {

namespace video_renderer {

namespace {
struct Sharing {
  sptr<VideoDecoderRef> decoder_ref;
};
}

class VideoClipPreRenderCommand : public RenderCommand {

private:
  int clip_id_;
  sptr<VideoResource> resource_;
  int64_t timestamp_;
  bool iframe_;

public:
  sptr<Sharing> sharing;

  inline VideoClipPreRenderCommand(
    int clip_id,
    sptr<VideoResource> resource,
    int64_t timestamp,
    bool iframe = false) :
    clip_id_(clip_id), resource_(resource), timestamp_(timestamp), iframe_(iframe) {
    sharing = sptr<Sharing>(new Sharing());
  }

  inline void render(sptr<RendererContext> ctx) {
// std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    int width = resource_->width();
    int height = resource_->height();
    auto rt = ctx->createTemporaryRenderTexture("clip", width, height);
    auto rtt = ctx->createTemporaryRenderTexture("clip_temp", width, height);
    auto decoder_ref = ctx->decoder_manager()->acquireDecoder(resource_, clip_id_);
    sharing->decoder_ref = decoder_ref;
    sptr<VideoFrame> video_frame = decoder_ref->decoder()->decode(timestamp_, iframe_);
    if (!video_frame) return;
// std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
// qDebug() << "Decode = " <<  sec.count();
    SwsKey sws_key;
    sws_key.src_width = width;
    sws_key.src_height = height;
    sws_key.out_width = width;
    sws_key.out_height = height;
    sws_key.src_fmt = video_frame->pix_fmt();
    auto sws_context = ctx->getSwsValue(sws_key);
    auto frame = video_frame->frame();
    int linesize[4] = { width * 4, 0, 0, 0 };
    uint8_t* data[4] = { sws_context->out_data, 0, 0, 0 };
    sws_scale(sws_context->context, frame->data, frame->linesize,
      0, height, data, linesize);
// sec = std::chrono::system_clock::now() - start;
// qDebug() << "Scale = " <<  sec.count();  
    auto gf = ctx->gl()->functions();
    gf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    gf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sws_context->out_data);
    gf->glBindTexture(GL_TEXTURE_2D, 0);
// sec = std::chrono::system_clock::now() - start;
// qDebug() << "PreEnd = " <<  sec.count();  
  }

};

class VideoClipPostRenderCommand : public RenderCommand {

private:
  sptr<VideoResource> resource_;
  int64_t timestamp_;

public:
  sptr<Sharing> sharing;

  inline VideoClipPostRenderCommand(
    sptr<Sharing> sharing,
    sptr<VideoResource> resource) :
    sharing(sharing), resource_(resource) {
  }

  inline void render(sptr<RendererContext> ctx) {
    ctx->decoder_manager()->releaseDecoder(sharing->decoder_ref);
    ctx->releaseTemporaryRenderTexture("clip");
    ctx->releaseTemporaryRenderTexture("clip_temp");
  }

};

}

}

#endif