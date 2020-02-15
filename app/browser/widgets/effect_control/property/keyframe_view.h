#ifndef NLIVE_EFFECT_CONTROL_KEYFRAME_VIEW_H_
#define NLIVE_EFFECT_CONTROL_KEYFRAME_VIEW_H_

#include "base/common/memory.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include "base/layout/div.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "browser/widgets/effect_control/keyframes_controller.h"

namespace nlive {

namespace effect_control {

class KeyframeView : public Div {

private:
  sptr<IThemeService> theme_service_;
  sptr<IKeyframesController> keyframes_controller_;
  SequenceScrollView* sequence_scroll_view_;

  sptr<effect::IKeyframe> keyframe_;
  bool active_;

  int64_t lastTime_;

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
    p.fillPath(path, !active_ ?
        theme.surfaceBrightColor() : theme.primaryColor());
  }

  void mousePressEvent(QMouseEvent* e) override {
    onMousePress();
    lastTime_ = sequence_scroll_view_->getTimeRelativeToView(e->globalX());
  }

  void mouseMoveEvent(QMouseEvent* e) override {
    int64_t curTime_ = sequence_scroll_view_->getTimeRelativeToView(e->globalX());
    if (lastTime_ != curTime_) {
      int dTime = curTime_ - lastTime_;
      keyframes_controller_->translate(dTime);
      lastTime_ = curTime_;
    }
  }

public:
  KeyframeView(QWidget* parent,
    sptr<IKeyframesController> keyframes_controller,
    sptr<effect::IKeyframe> keyframe,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service) :
      Div(parent), theme_service_(theme_service),
      keyframes_controller_(keyframes_controller),
      sequence_scroll_view_(sequence_scroll_view),
      keyframe_(keyframe), active_(false) {

  }
  
  void setActive(bool value) {
    active_ = value;
    update();
  }

  sptr<effect::IKeyframe> keyframe() { return keyframe_; }

  sig2_t<void ()> onMousePress;

};

}

}

#endif