#ifndef NLIVE_DIV_H_
#define NLIVE_DIV_H_

#include <QWidget>
#include "base/common/sig.h"

namespace nlive {

class Div : public QWidget, public Sig {

private:
  float flex_;
  float flex_grow_;
  float flex_shrink_;
  int flex_basis_;

public:
  inline Div(QWidget* parent) :
    QWidget(parent),
    flex_(1), flex_grow_(1), flex_shrink_(1), flex_basis_(0) {

  }

  inline Div* setFlex(float value) {
    flex_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline Div* setFlexGrow(float value) {
    flex_grow_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline Div* setFlexShrink(float value) {
    flex_shrink_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline Div* setFlexBasis(int value) {
    flex_basis_ = value;
    onFlexUpdate();
    updateGeometry();
    return this;
  }
  inline float flex() const { return flex_; }
  inline float flex_grow() const { return flex_grow_; }
  inline float flex_shrink() const { return flex_shrink_; }
  inline int flex_basis() const { return flex_basis_; }

  sig2_t<void()> onFlexUpdate;

};

}

#endif