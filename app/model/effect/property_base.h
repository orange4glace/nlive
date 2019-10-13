#ifndef NILVE_EFFECT_PROPERTY_BASE_H_
#define NILVE_EFFECT_PROPERTY_BASE_H_

#include <QObject>
#include <stdint.h>
#include <map>
#include <string>

#include "base/common/sig.h"

namespace nlive {

namespace effect {

template <class T>
class Property {
  
private:
  std::string type_;

  bool animatable_;
  bool animated_;

public:
  PropertyBase(bool animatable = true) :
    animatable_(animatable), animated_(false) {

  }

  void setAnimatable(bool value) {
    animatable_ = value;
    onDidChangeAnimatable(value);
    onDidUpdate();
  }

  const std::string& type() const {
    return type_;
  }

  bool animatable() const {
    return animatable_;
  }

  bool animated() const {
    return animated_;
  }

  sig2_t<void (bool)> onDidChangeAnimated;
  sig2_t<void (bool)> onDidChangeAnimatable;
  sig2_t<void (void)> onDidUpdate;

};

}

}

#endif