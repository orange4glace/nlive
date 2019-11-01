#ifndef NLIVE_EFFECT_CONTROL_LAYOUT_H_
#define NLIVE_EFFECT_CONTROL_LAYOUT_H_

#include "base/common/sig.h"

namespace nlive {

namespace effect_control {

class EffectControlLayout : public Sig {

private:
  int width_;
  int form_value_width_;
  int form_width_;
  int timeline_width_;
  double side_timeline_ratio_;

  inline void recalculate() {
    timeline_width_ = width_ * side_timeline_ratio_;
    form_width_ = width_ - timeline_width_;
    onDidUpdate();
  }

public:
  inline EffectControlLayout(double side_timeline_ratio) :
    side_timeline_ratio_(side_timeline_ratio) {
    recalculate();
  }

  inline void setFormValueWidth(int value) {
    form_value_width_ = value;
    onDidUpdate();
  }

  inline void setTimelineRatio(double value) {
    side_timeline_ratio_ = value;
    recalculate();
  }

  inline void setWidth(int value) {
    width_ = value;
    recalculate();
  }

  inline int width() const { return width_; }
  inline int form_value_width() const { return form_value_width_; }
  inline double side_timeline_ratio() const { return side_timeline_ratio_; }
  inline int form_width() const { return form_width_; }
  inline int timeline_width() const { return timeline_width_; }

  sig2_t<void ()> onDidUpdate;

};

}

}

#endif