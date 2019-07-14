#ifndef _NLIVE_RATIONAL_H_
#define _NLIVE_RATIONAL_H_

#include <stdint.h>
extern "C" {
#include <libavutil/rational.h>
}

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
  int num_;
  int den_;

public:
  Rational(int num, int den);
  Rational(const Rational& from);

  void setNum(int value);
  void setDen(int value);

  int num() const;
  int den() const;

  static int64_t rescale(int a, const Rational& from, const Rational& to, Rounding rounding = Rounding::ROUND_NEAR_INF);
  static Rational fromAVRational(const AVRational& rational);

};

}

#endif