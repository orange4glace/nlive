#ifndef NLIVE_PLATFORM_ENCODER_ENCODING_SERVICE_H_
#define NLIVE_PLATFORM_ENCODER_ENCODING_SERVICE_H_

#include "base/common/memory.h"
#include "platform/include.h"
#include "model/include.h"
#include "renderer/video_renderer/surface_provider.h"

namespace nlive {

class IEncodingService {

public:
  virtual sptr<Task> encode(sptr<Sequence> sequence) = 0;

};

}

#endif