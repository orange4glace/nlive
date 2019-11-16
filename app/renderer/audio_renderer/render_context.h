#ifndef NLIVE_AUDIO_RENDERER_RENDER_CONTEXT_H_
#define NLIVE_AUDIO_RENDERER_RENDER_CONTEXT_H_

extern "C" {
  #include <libswresample/swresample.h>
}
#include <map>
#include "renderer/audio_renderer/decoder_manager.h"

namespace nlive {

namespace audio_renderer {

struct SwrKey {
  int64_t in_ch_layout;
  AVSampleFormat in_sample_fmt;
  int in_sample_rate;
};

struct SwrKeyCompare {
  inline bool operator() (const SwrKey& a, const SwrKey& b) const {
    if (a.in_ch_layout == b.in_ch_layout) {
      if (a.in_sample_fmt == b.in_sample_fmt) {
        return a.in_sample_rate < b.in_sample_rate;
      }
      return a.in_sample_fmt < b.in_sample_fmt;
    }
    return a.in_ch_layout < b.in_ch_layout;
  }
};

struct SwrValue {
  SwrContext* context;
  uint8_t* out_data;
  uint8_t* flush_data;
};

class RenderContext {

private:
  int64_t ch_layout_;
  int nb_channels_;
  AVSampleFormat sample_fmt_;
  int sample_rate_;
  int samples_per_channel_;
  int bytes_per_channel_;
  int bytes_per_sample_;
  int buffer_size_;

  // Audio data is interleaved.
  uint8_t* data_;

  std::map<SwrKey, SwrValue, SwrKeyCompare> swr_map_;

  QSharedPointer<DecoderManager> decoder_manager_;

  SwrValue* getSwrValue(SwrKey& key);

public:
  RenderContext(int64_t ch_layout, AVSampleFormat sample_fmt, int sample_rate, int samples_per_channel_);
  ~RenderContext();

  void clearData();
  uint8_t** resample(int64_t in_ch_layout, AVSampleFormat in_sample_fmt, int in_sample_rate,
      const uint8_t** in_data, int in_samples);

  inline int64_t ch_layout() const { return ch_layout_; }
  inline int nb_channels() const { return nb_channels_; }
  inline AVSampleFormat sample_fmt() const { return sample_fmt_; }
  inline int sample_rate() const { return sample_rate_; }
  inline int samples_per_channel() const { return samples_per_channel_; }
  inline int bytes_per_channel() const { return bytes_per_sample_ * samples_per_channel_; }
  inline int bytes_per_sample() const { return bytes_per_sample_; }
  inline int buffer_size() const { return buffer_size_; }
  inline int bytes_per_sample() { return bytes_per_sample_; }
  inline uint8_t* data() { return data_; }
  inline QSharedPointer<DecoderManager> decoder_manager() { return decoder_manager_; }

};

}

}

#endif