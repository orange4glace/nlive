#ifndef NLIVE_EFFECT_CONTROL_KEYFRAME_VIEW_H_
#define NLIVE_EFFECT_CONTROL_KEYFRAME_VIEW_H_

#include "base/common/memory.h"
#include <QPainter>
#include <QPainterPath>
#include "base/layout/div.h"
#include "platform/theme/themeservice.h"

namespace nlive {

namespace effect_control {

template <class T>
class KeyframeView : public Div {

private:
  sptr<IThemeService> theme_service_;

  sptr<effect::Keyframe<T>> keyframe_;
  bool active_;

protected:
  void paintEvent(QPaintEvent* e) override {
    QPainter p(this);
    auto& theme = theme_service_->getTheme();
    auto& r = rect();
    auto c = r.center();
    QPainterPath path;
    path.moveTo(c.x(), r.top());
    path.lineTo(r.right(), c.y());
    path.lineTo(c.x(), r.bottom());
    path.lineTo(r.left(), c.y());
    p.fillPath(path, theme.surfaceBrightColor());
  }

public:
  KeyframeView(QWidget* parent, sptr<effect::Keyframe<T>> keyframe,
    sptr<IThemeService> theme_service) :
    Div(parent), theme_service_(theme_service),
    keyframe_(keyframe) {

  }
  
  void setActive(bool value) {
    active_ = value;
    update();
  }

  sptr<effect::Keyframe<T>> keyframe() { return keyframe_; }

};

}

}

#endif