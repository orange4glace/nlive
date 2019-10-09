#include "model/resource/video_resource.h"

namespace nlive {

const std::string VideoResource::TYPE = "nlive.Resource.VideoResource";

VideoResource::VideoResource(
  std::string path, Rational time_base, Rational frame_rate, int64_t duration,
  int width, int height) :
  Resource(VideoResource::TYPE, path),
  time_base_(time_base), frame_rate_(frame_rate), duration_(duration),
  width_(width), height_(height) {

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

int VideoResource::width() const {
  return width_;
}

int VideoResource::height() const {
  return height_;
}

}