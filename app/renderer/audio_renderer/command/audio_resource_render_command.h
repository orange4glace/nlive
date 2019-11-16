#ifndef NLIVE_AUDIO_RENDERER_AUDIO_RESOURCE_RENDER_COMMAND_H_
#define NLIVE_AUDIO_RENDERER_AUDIO_RESOURCE_RENDER_COMMAND_H_

#include <QDebug>
#include <cstring>
#include "renderer/audio_renderer/render_command.h"
#include "model/resource/audio_resource.h"
#include "model/resource/raw_audio_resource.h"

namespace nlive {

namespace audio_renderer {

class AudioResourceRenderCommand : public RenderCommand {

private:
  QSharedPointer<AudioResource> resource_;
  QSharedPointer<RawAudioResource> raw_resource_;
  int out_sample_rate_;
  int clip_id_;
  int64_t start_frame_;
  int64_t end_frame_;

public:
  inline AudioResourceRenderCommand(QSharedPointer<AudioResource> resource, 
      int out_sample_rate, int clip_id, int64_t start_frame, int64_t end_frame) :
      resource_(resource), out_sample_rate_(out_sample_rate), clip_id_(clip_id), 
      start_frame_(start_frame), end_frame_(end_frame) {
    raw_resource_ = resource_->raw();
  }

  inline void render(QSharedPointer<RenderContext> ctx) override {
    if (!raw_resource_) return;
    int resampled_start_frame = start_frame_ * raw_resource_->sample_rate() / out_sample_rate_;
    int resampled_end_frame = end_frame_ * raw_resource_->sample_rate() / out_sample_rate_;
    auto dec_manager = ctx->decoder_manager();
    auto dec = dec_manager->acquireDecoder(raw_resource_, clip_id_);
    auto frame = dec->decoder()->decode(resampled_start_frame, resampled_end_frame);
    auto data = frame->data();
    uint8_t** out_data = ctx->resample(raw_resource_->ch_layout(),
        raw_resource_->sample_format(), raw_resource_->sample_rate(),
        const_cast<const uint8_t**>(&data), resampled_end_frame - resampled_start_frame);
    // qDebug() << data_pointer_[0] << out_data[0][12799] << ctx->buffer_size();
    memcpy(ctx->data(), out_data[0], ctx->buffer_size());
  }

};

}

}

#endif