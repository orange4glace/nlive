#ifndef NLIVE_RAW_AUDIO_RESOUCE_H_
#define NLIVE_RAW_AUDIO_RESOUCE_H_

#include <string>

extern "C" {
#include <libswresample/swresample.h>
}
#include "model/resource/resource.h"
#include "model/common/rational.h"

namespace nlive {

class RawAudioResource : public Resource {

private:
  int64_t ch_layout_;
  AVSampleFormat sample_format_;
  int nb_channels_;
  int bytes_per_sample_;
  Rational time_base_;
  int64_t sample_rate_;
  int64_t nb_frames_;
  bool planar_;

public:
  static const std::string TYPE;

  RawAudioResource(std::string path, int64_t ch_layout, AVSampleFormat sample_format, int sample_rate, int64_t nb_frames);

  inline int64_t ch_layout() const { return ch_layout_; }
  inline AVSampleFormat sample_format() const { return sample_format_; }
  inline int nb_channels() const { return nb_channels_; }
  inline int bytes_per_sample() const { return bytes_per_sample_; }
  inline const Rational& time_base() const { return time_base_; }
  inline int sample_rate() const { return sample_rate_; }
  inline int64_t nb_frames() const { return nb_frames_; }
  inline bool planar() const { return planar_; }

};

}

#endif