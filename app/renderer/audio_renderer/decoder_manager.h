#ifndef NLIVE_AUDIO_RENDERER_DECODER_MANAGER_H_
#define NLIVE_AUDIO_RENDERER_DECODER_MANAGER_H_

#include <map>
#include <vector>
#include "base/common/memory.h"

#include "model/resource/raw_audio_resource.h"
#include "decoder/audio_decoder.h"

namespace nlive {

namespace audio_renderer {

class DecoderManager;

class AudioDecoderRef {
  private:
    sptr<AudioDecoder> decoder_;
    sptr<RawAudioResource> resource_;
    int referer_;
    int freq_;

    friend class DecoderManager;

  public:
    inline AudioDecoderRef(
      sptr<AudioDecoder> decoder, 
      sptr<RawAudioResource> resource,
      int referer) : decoder_(decoder), resource_(resource), referer_(referer),
      freq_(0) {

    }

    inline sptr<AudioDecoder> decoder() { return decoder_; }
    inline sptr<RawAudioResource> resource() { return resource_; }
    inline int referer() const { return referer_; }
    inline int freq() const { return freq_; }
};

class DecoderManager {

private:
  std::vector<sptr<AudioDecoderRef>> decoder_refs_;
  
public:
  DecoderManager();

  sptr<AudioDecoderRef> acquireDecoder(
      sptr<RawAudioResource> resource, int referer);

  void releaseDecoder(sptr<AudioDecoderRef> decoder_ref);

};

}

}

#endif