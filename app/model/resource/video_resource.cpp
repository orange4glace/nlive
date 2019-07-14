#include "model/resource/video_resource.h"

namespace nlive {

const QString VideoResource::TYPE = "nlive.Resource.VideoResource";

VideoResource::VideoResource(QUrl path, Rational time_base, Rational frame_rate, int64_t duration) :
  Resource(VideoResource::TYPE, path),
  time_base_(time_base), frame_rate_(frame_rate), duration_(duration) {

}

const Rational& VideoResource::time_base() const {
  return time_base_;
}

const Rational& VideoResource::frame_rate() const {
  return frame_rate_;
}

int64_t VideoResource::duration() const {
  return duration_;
}

}