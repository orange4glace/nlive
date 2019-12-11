#ifndef NLIVE_PLATFORM_ENCODER_ENCODING_SERVICE_H_
#define NLIVE_PLATFORM_ENCODER_ENCODING_SERVICE_H_

#include <QSharedPointer>
#include "base/common/memory.h"
#include "platform/service/service.h"
#include "platform/include.h"
#include "model/include.h"
#include "renderer/video_renderer/surface_provider.h"

namespace nlive {

class IEncodingService : public IService {
DECLARE_SERVICE("nlive.platform.services.EncodingService")

public:
  virtual QSharedPointer<Task> encode(sptr<Sequence> sequence, std::wstring filename) = 0;

};

}

#endif