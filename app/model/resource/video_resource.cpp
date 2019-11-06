#include "model/resource/video_resource.h"

#include <QDebug>
#include <boost/filesystem.hpp>

namespace nlive {

const std::string VideoResource::TYPE = "nlive.Resource.VideoResource";

VideoResource::VideoResource(
  std::string path, Rational time_base, Rational frame_rate, int64_t duration,
  int width, int height) :
  Resource(VideoResource::TYPE, path,
      QString::fromStdString(boost::filesystem::path(path).filename().string())),
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

int64_t VideoResource::duration_in_seconds() const {
  return duration_ * time_base_.num() / time_base_.den();
}

int VideoResource::width() const {
  return width_;
}

int VideoResource::height() const {
  return height_;
}

}