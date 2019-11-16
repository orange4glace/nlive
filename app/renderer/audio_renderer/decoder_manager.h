#ifndef NLIVE_AUDIO_RENDERER_DECODER_MANAGER_H_
#define NLIVE_AUDIO_RENDERER_DECODER_MANAGER_H_

#include <map>
#include <vector>
#include <QSharedPointer>

#include "model/resource/raw_audio_resource.h"
#include "decoder/audio_decoder.h"

namespace nlive {

namespace audio_renderer {

class DecoderManager;

class AudioDecoderRef {
  private:
    QSharedPointer<AudioDecoder> decoder_;
    QSharedPointer<RawAudioResource> resource_;
    int referer_;
    int freq_;

    friend class DecoderManager;

  public:
    inline AudioDecoderRef(
      QSharedPointer<AudioDecoder> decoder, 
      QSharedPointer<RawAudioResource> resource,
      int referer) : decoder_(decoder), resource_(resource), referer_(referer),
      freq_(0) {

    }

    inline QSharedPointer<AudioDecoder> decoder() { return decoder_; }
    inline QSharedPointer<RawAudioResource> resource() { return resource_; }
    inline int referer() const { return referer_; }
    inline int freq() const { return freq_; }
};

class DecoderManager {

private:
  std::vector<QSharedPointer<AudioDecoderRef>> decoder_refs_;
  
public:
  DecoderManager();

  QSharedPointer<AudioDecoderRef> acquireDecoder(
      QSharedPointer<RawAudioResource> resource, int referer);

  void releaseDecoder(QSharedPointer<AudioDecoderRef> decoder_ref);

};

}

}

#endif