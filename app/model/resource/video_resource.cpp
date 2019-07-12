#include "model/resource/video_resource.h"

namespace nlive {

VideoResource::VideoResource(QUrl path, Timebase timebase, int duration) :
  Resource("nlive.VideoResource", path),
  timebase_(timebase), duration_(duration) {

}

const Timebase& VideoResource::timebase() const {
  return timebase_;
}

int64_t VideoResource::duration() const {
  return duration_;
}

}