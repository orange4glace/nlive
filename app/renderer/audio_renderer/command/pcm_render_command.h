#ifndef NLIVE_AUDIO_RENDERER_PCM_RENDER_COMMAND_H_
#define NLIVE_AUDIO_RENDERER_PCM_RENDER_COMMAND_H_

#include <QDebug>
#include <cstring>
#include "renderer/audio_renderer/render_command.h"

namespace nlive {

namespace audio_renderer {

class PCMRenderCommand : public RenderCommand {

private:
  uint8_t* data_;
  uint8_t* data_pointer_;
  int64_t ch_layout_;
  AVSampleFormat sample_fmt_;
  int sample_rate_;
  int samples_;

public:
  inline PCMRenderCommand(int64_t ch_layout, AVSampleFormat sample_fmt, int sample_rate, int samples,
      uint8_t* data, int sample_offset) :
    data_(data), ch_layout_(ch_layout), sample_fmt_(sample_fmt),
    sample_rate_(sample_rate), samples_(samples) {
    data_pointer_ = data_ + sample_offset;
    int nb_channels = av_get_channel_layout_nb_channels(ch_layout);
    int bytes_per_sample_ = av_get_bytes_per_sample((AVSampleFormat)sample_fmt);
  }

  inline void render(QSharedPointer<RenderContext> ctx) override {
    uint8_t** out_data = ctx->resample(ch_layout_, sample_fmt_, sample_rate_, const_cast<const uint8_t**>(&data_pointer_), samples_);
    auto data = ctx->data();
    // qDebug() << data_pointer_[0] << out_data[0][12799] << ctx->buffer_size();
    memcpy(data, out_data[0], ctx->buffer_size());
  }

};

}

}

#endif