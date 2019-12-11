#include "renderer/audio_renderer/render_context.h"

#include <QDebug>
#include <cstring>
#include <stdint.h>
#include <assert.h>

namespace nlive {

namespace audio_renderer {

RenderContext::RenderContext(int64_t ch_layout, AVSampleFormat sample_fmt,
    int sample_rate, int samples_per_channel) :
    ch_layout_(ch_layout), sample_fmt_(sample_fmt), sample_rate_(sample_rate),
    samples_per_channel_(samples_per_channel) {
  nb_channels_ = av_get_channel_layout_nb_channels(ch_layout);
  bytes_per_sample_ = av_get_bytes_per_sample((AVSampleFormat)sample_fmt);
  bytes_per_channel_ = samples_per_channel_ * bytes_per_sample_;
  buffer_size_ = nb_channels_ * bytes_per_channel_;
  data_ = new uint8_t[buffer_size_];
  decoder_manager_ = sptr<DecoderManager>(new DecoderManager());
  clearData();
}

RenderContext::~RenderContext() {
  for (auto& kv : swr_map_) {
    SwrContext* ctx = kv.second.context;
    uint8_t* data = kv.second.out_data;
    swr_free(&ctx);
    av_freep(&data);
    av_freep(&kv.second.flush_data);
  }
  delete data_;
}

SwrValue* RenderContext::getSwrValue(SwrKey& key) {
  if (swr_map_.count(key)) return &swr_map_[key];
  SwrValue val;
  val.context = swr_alloc_set_opts(NULL,
    ch_layout_, sample_fmt_, sample_rate_,
    key.in_ch_layout, key.in_sample_fmt, key.in_sample_rate,
    0, NULL);
  swr_init(val.context);
  uint8_t* out_data;
  uint8_t* flush_data;
  int linesize;
  av_samples_alloc(&out_data, &linesize, nb_channels_, samples_per_channel_ + 20000 /* padding for safety */, sample_fmt_, 0);
  av_samples_alloc(&flush_data, &linesize, nb_channels_, samples_per_channel_ + 20000, sample_fmt_, 0);
  val.out_data = out_data;
  val.flush_data = flush_data;
  swr_map_[key] = val;
  return &swr_map_[key];
}

uint8_t** RenderContext::resample(int64_t in_ch_layout, AVSampleFormat in_sample_fmt, int in_sample_rate,
    const uint8_t** in_data, int in_samples) {
  // assert(in_samples == samples_per_channel_);
  SwrKey key;
  key.in_ch_layout = in_ch_layout;
  key.in_sample_fmt = in_sample_fmt;
  key.in_sample_rate = in_sample_rate;
  SwrValue* swr = getSwrValue(key);
  swr_convert(swr->context, &swr->out_data, samples_per_channel_, in_data, in_samples);
  swr_convert(swr->context, &swr->flush_data, samples_per_channel_, 0, 0);
  return &swr->out_data;
}

void RenderContext::clearData() {
  memset(data_, 0, buffer_size_);
}

}

}