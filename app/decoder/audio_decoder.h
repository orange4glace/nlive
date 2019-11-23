#ifndef NLIVE_AUDIO_DECODER_H_
#define NLIVE_AUDIO_DECODER_H_

#include "base/common/memory.h"
#include <QFile>
#include <string>
#include "model/resource/raw_audio_resource.h"
#include "decoder/audio_frame.h"

namespace nlive {

class AudioDecoder {

private:
  sptr<RawAudioResource> resource_;
  QFile file_;
  sptr<AudioFrame> frame_;

public:
  AudioDecoder(sptr<RawAudioResource> resource);

  sptr<AudioFrame> decode(int64_t start_frame, int64_t end_frame);

  inline sptr<RawAudioResource> resource() { return resource_; }

};

}

#endif