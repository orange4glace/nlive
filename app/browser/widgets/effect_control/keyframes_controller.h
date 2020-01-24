#ifndef NLIVE_BROWSER_EFFECT_CONTROL_KEYFRAMES_CONTROLLER_H_
#define NLIVE_BROWSER_EFFECT_CONTROL_KEYFRAMES_CONTROLLER_H_

#include <stdint.h>

namespace nlive {

namespace effect_control {

class IKeyframesController {

public:
  virtual void translate(int64_t time) = 0;
  virtual void remove() = 0;

};

}

}

#endif