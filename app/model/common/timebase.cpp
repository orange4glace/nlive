#include "model/common/timebase.h"

extern "C" {
#include <libavutil/mathematics.h>
}

namespace nlive {

Timebase::Timebase(int num, int den) :
  num_(num), den_(den) {

}

Timebase::Timebase(const Timebase& from) :
  Timebase(from.num_, from.den_) {
  
}

void Timebase::setNum(int value) {
  num_ = value;
}

void Timebase::setDen(int value) {
  den_ = value;
}

int Timebase::num() const {
  return num_;
}

int Timebase::den() const {
  return den_;
}

int64_t Timebase::rescale(int a, const Timebase& from, const Timebase& to, Rounding rounding) {
  return av_rescale_q(a, { to.num_, to.den_ }, { from.num_, from.den_ });
}

}