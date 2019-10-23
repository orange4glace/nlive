#ifndef _NLIVE_VIDEO_RESOUCE_H_
#define _NLIVE_VIDEO_RESOUCE_H_

#include <QSharedPointer>
#include <string>
#include <mutex>

#include "model/resource/resource.h"
#include "model/common/rational.h"
#include "decoder/video_decoder.h"

namespace nlive {

class VideoResource : public Resource {
  Q_OBJECT

private:
  Rational time_base_;
  Rational frame_rate_;
  int64_t duration_;
  int width_;
  int height_;

public:
  static const std::string TYPE;

  VideoResource(std::string path, Rational time_base, Rational frame_rate, int64_t duration,
    int width, int height);

  const Rational& time_base() const;
  const Rational& frame_rate() const;
  int64_t duration() const;
  int width() const;
  int height() const;

};

}

#endif