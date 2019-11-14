#include "model/common/rational.h"

extern "C" {
#include <libavutil/mathematics.h>
}

namespace nlive {

Rational::Rational(int num, int den) :
  num_(num), den_(den) {

}

Rational::Rational(const Rational& from) :
  Rational(from.num_, from.den_) {
  
}

void Rational::setNum(int value) {
  num_ = value;
}

void Rational::setDen(int value) {
  den_ = value;
}

int Rational::num() const {
  return num_;
}

int Rational::den() const {
  return den_;
}

int64_t Rational::rescale(int64_t a, const Rational& from, const Rational& to, Rounding rounding) {
  // TODO
  return av_rescale_q(a, { from.num_, from.den_ }, { to.num_, to.den_ });
}

int64_t Rational::rescale(int64_t a, int64_t b, int64_t c, Rounding rounding) {
  // TODO
  return av_rescale_rnd(a, b, c, AVRounding::AV_ROUND_DOWN);
}

int64_t Rational::rescale(int64_t a, const Rational& b, Rounding rounding) {
  // TODO
  return rescale(a, b.num(), b.den(), rounding);
}

Rational Rational::fromAVRational(const AVRational& rational) {
  return Rational(rational.num, rational.den);
}

}