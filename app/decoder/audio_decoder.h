#ifndef NLIVE_AUDIO_DECODER_H_
#define NLIVE_AUDIO_DECODER_H_

#include <QSharedPointer>
#include <QFile>
#include <string>
#include "model/resource/raw_audio_resource.h"
#include "decoder/audio_frame.h"

namespace nlive {

class AudioDecoder {

private:
  QSharedPointer<RawAudioResource> resource_;
  QFile file_;
  QSharedPointer<AudioFrame> frame_;

public:
  AudioDecoder(QSharedPointer<RawAudioResource> resource);

  QSharedPointer<AudioFrame> decode(int64_t start_frame, int64_t end_frame);

  inline QSharedPointer<RawAudioResource> resource() { return resource_; }

};

}

#endif