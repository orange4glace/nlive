#ifndef NLIVE_AUDIO_RESOUCE_H_
#define NLIVE_AUDIO_RESOUCE_H_

#include <string>

#include "model/resource/resource.h"
#include "model/common/rational.h"

namespace nlive {

class AudioResource : public Resource {

private:
  std::string raw_path_;
  int64_t sample_rate_;
  int64_t duration_;

public:
  static const std::string TYPE;

  AudioResource(std::string path);

  int64_t sample_rate() const;
  int64_t duration() const;

};

}

#endif