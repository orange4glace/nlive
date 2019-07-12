#ifndef _NLIVE_VIDEO_RESOUCE_H_
#define _NLIVE_VIDEO_RESOUCE_H_

#include "model/resource/resource.h"
#include "model/common/timebase.h"

namespace nlive {

class VideoResource : public Resource {
  Q_OBJECT

private:
  Timebase timebase_;
  int64_t duration_;

public:
  VideoResource(QUrl path, Timebase timebase, int duration);

  const Timebase& timebase() const;
  int64_t duration() const;

};

}

#endif