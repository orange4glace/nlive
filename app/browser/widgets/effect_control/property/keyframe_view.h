#ifndef NLIVE_EFFECT_CONTROL_KEYFRAME_VIEW_H_
#define NLIVE_EFFECT_CONTROL_KEYFRAME_VIEW_H_

#include <QSharedPointer>
#include <QPainter>
#include <QPainterPath>
#include "base/layout/div.h"
#include "platform/theme/themeservice.h"

namespace nlive {

namespace effect_control {

template <class T>
class KeyframeView : public Div {

private:
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<effect::Keyframe<T>> keyframe_;
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
  KeyframeView(QWidget* parent, QSharedPointer<effect::Keyframe<T>> keyframe,
    QSharedPointer<IThemeService> theme_service) :
    Div(parent), theme_service_(theme_service),
    keyframe_(keyframe) {

  }
  
  void setActive(bool value) {
    active_ = value;
    update();
  }

  QSharedPointer<effect::Keyframe<T>> keyframe() { return keyframe_; }

};

}

}

#endif