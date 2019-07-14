#ifndef _NLIVE_VIDEO_RESOUCE_H_
#define _NLIVE_VIDEO_RESOUCE_H_

#include "model/resource/resource.h"
#include "model/common/rational.h"

namespace nlive {

class VideoResource : public Resource {
  Q_OBJECT

private:
  Rational time_base_;
  Rational frame_rate_;
  int64_t duration_;

public:
  static const QString TYPE;

  VideoResource(QUrl path, Rational time_base, Rational frame_rate, int64_t duration);

  const Rational& time_base() const;
  const Rational& frame_rate() const;
  int64_t duration() const;

};

}

#endif