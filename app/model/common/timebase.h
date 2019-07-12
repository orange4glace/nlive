#ifndef _NLIVE_TIMEBASE_H_
#define _NLIVE_TIMEBASE_H_

#include <stdint.h>

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

class Timebase {

private:
  int num_;
  int den_;

public:
  Timebase(int num, int den);
  Timebase(const Timebase& from);

  void setNum(int value);
  void setDen(int value);

  int num() const;
  int den() const;

  static int64_t rescale(int a, const Timebase& from, const Timebase& to, Rounding rounding = Rounding::ROUND_NEAR_INF);

};

}

#endif