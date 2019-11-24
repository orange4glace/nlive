#ifndef NLIVE_RATIONAL_H_
#define NLIVE_RATIONAL_H_

#include <stdint.h>
extern "C" {
#include <libavutil/rational.h>
}
#include "base/common/serialize.h"

namespace nlive {

/**
 * Rounding rules
 * See FFmpeg AVRounding
 */
enum Rounding {
  ROUND_ZERO = 0,
  ROUND_INF = 1,
  ROUND_DOWN = 2,
  ROUND_UP = 3,
  ROUND_NEAR_INF = 5,
  ROUND_PASS_MINMAX = 8192,
};

class Rational {

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & num_ & den_;
  }

  int num_;
  int den_;

public:
  Rational();
  Rational(int num, int den);
  Rational(const Rational& from);

  void setNum(int value);
  void setDen(int value);

  int num() const;
  int den() const;

  static int64_t rescale(int64_t a, const Rational& from, const Rational& to, Rounding rounding = Rounding::ROUND_NEAR_INF);
  static int64_t rescale(int64_t a, int64_t b, int64_t c, Rounding rounding = Rounding::ROUND_NEAR_INF);
  static int64_t rescale(int64_t a, const Rational& b, Rounding rounding = Rounding::ROUND_NEAR_INF);
  static Rational fromAVRational(const AVRational& rational);

  inline bool operator==(const Rational &rhs) const {
    return num_ == rhs.num_ && den_ == rhs.den_;
  }

};

}

#endif