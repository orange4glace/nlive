#ifndef NLIVE_AUDIO_DECODER_H_
#define NLIVE_AUDIO_DECODER_H_

#include <string>

namespace nlive {

class AudioDecoder {

private:
  std::string path_;

public:
  AudioDecoder(std::string path);

};

}

#endif