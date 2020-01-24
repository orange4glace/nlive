#ifndef NLIVE_BROWSER_EFFECT_CONTROL_KEYFRAMES_CONTROLLER_IMPL_H_
#define NLIVE_BROWSER_EFFECT_CONTROL_KEYFRAMES_CONTROLLER_IMPL_H_

#include "browser/widgets/effect_control/keyframes_controller.h"

#include "browser/widgets/effect_control/effect_control_widget.h"

namespace nlive {

namespace effect_control {

class KeyframesController : public IKeyframesController {

private:
  EffectControlWidget* widget_;

public:
  KeyframesController(EffectControlWidget* widget);

  void translate(int64_t time) override;
  void remove() override;

};

}

}

#endif