#ifndef NLIVE_VIDEO_CLIP_RENDER_COMMAND_H_
#define NLIVE_VIDEO_CLIP_RENDER_COMMAND_H_

#include <QSharedPointer>
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
  QSharedPointer<VideoDecoderRef> decoder_ref;
};
}

class VideoClipPreRenderCommand : public RenderCommand {

private:
  int clip_id_;
  QSharedPointer<VideoResource> resource_;
  int64_t timestamp_;
  bool iframe_;

public:
  QSharedPointer<Sharing> sharing;

  inline VideoClipPreRenderCommand(
    int clip_id,
    QSharedPointer<VideoResource> resource,
    int64_t timestamp,
    bool iframe = false) :
    clip_id_(clip_id), resource_(resource), timestamp_(timestamp), iframe_(iframe) {
    sharing = QSharedPointer<Sharing>(new Sharing());
  }

  inline void render(QSharedPointer<RendererContext> ctx) {
// std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    int width = resource_->width();
    int height = resource_->height();
    auto rt = ctx->createTemporaryRenderTexture("clip", width, height);
    auto rtt = ctx->createTemporaryRenderTexture("clip_temp", width, height);
    auto decoder_ref = ctx->decoder_manager()->acquireDecoder(resource_, clip_id_);
    sharing->decoder_ref = decoder_ref;
    QSharedPointer<VideoFrame> video_frame = decoder_ref->decoder()->decode(timestamp_, iframe_);
    if (!video_frame) return;
// std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
// qDebug() << "Decode = " <<  sec.count();  
    uint8_t* buf = new uint8_t[width * height * 4];
// sec = std::chrono::system_clock::now() - start;
// qDebug() << "Allocate = " <<  sec.count();  
    video_frame->scale((void*)buf);
// sec = std::chrono::system_clock::now() - start;
// qDebug() << "Scale = " <<  sec.count();  
    auto gf = ctx->gl()->functions();
    gf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    gf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
    gf->glBindTexture(GL_TEXTURE_2D, 0);
// sec = std::chrono::system_clock::now() - start;
// qDebug() << "PreEnd = " <<  sec.count();  
    delete buf;
  }

};

class VideoClipPostRenderCommand : public RenderCommand {

private:
  QSharedPointer<VideoResource> resource_;
  int64_t timestamp_;

public:
  QSharedPointer<Sharing> sharing;

  inline VideoClipPostRenderCommand(
    QSharedPointer<Sharing> sharing,
    QSharedPointer<VideoResource> resource) :
    sharing(sharing), resource_(resource) {
  }

  inline void render(QSharedPointer<RendererContext> ctx) {
    ctx->decoder_manager()->releaseDecoder(sharing->decoder_ref);
    ctx->releaseTemporaryRenderTexture("clip");
    ctx->releaseTemporaryRenderTexture("clip_temp");
  }

};

}

}

#endif