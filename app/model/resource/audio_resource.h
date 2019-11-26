#ifndef NLIVE_AUDIO_RESOUCE_H_
#define NLIVE_AUDIO_RESOUCE_H_

#include "base/common/memory.h"
#include <string>

#include "model/resource/resource.h"
#include "model/resource/raw_audio_resource.h"
#include "model/common/rational.h"

namespace nlive {

class AudioResource : public Resource {

private:  
  AudioResource() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Resource>(*this);
    ar & time_base_ & sample_rate_ & duration_;
  }

  Rational time_base_;
  int64_t sample_rate_;
  int64_t duration_;
  sptr<RawAudioResource> raw_;

public:
  static const std::string TYPE;

  AudioResource(std::string path, Rational time_base, int sample_rate, int64_t duration);

  void setRaw(sptr<RawAudioResource> raw);

  inline const Rational& time_base() const { return time_base_; }
  inline int sample_rate() const { return sample_rate_; }
  inline int64_t duration() const { return duration_; }
  inline int64_t duration_in_seconds() const {
    return duration_ * time_base_.num() / time_base_.den();
  }
  inline sptr<RawAudioResource> raw() { return raw_; }

};

}

#endif