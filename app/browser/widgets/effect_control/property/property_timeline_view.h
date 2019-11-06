#ifndef NLIVE_EFFECT_CONTROL_PROPERTY_TIMELINE_VIEW_H_
#define NLIVE_EFFECT_CONTROL_PROPERTY_TIMELINE_VIEW_H_

#include <QWidget>
#include <QPainter>
#include <map>
#include "base/layout/div.h"
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "browser/widgets/effect_control/property/keyframe_view.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/property.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

namespace effect_control {

template <class T>
class PropertyTimelineView : public Div {

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<EffectControlLayout> layout_params_;
  QSharedPointer<Sequence> sequence_;
  QSharedPointer<Clip> clip_;
  QSharedPointer<effect::Property<T>> property_;
  SequenceScrollView* sequence_scroll_view_;
  std::map<QSharedPointer<effect::Keyframe<T>>, KeyframeView<T>*> keyframe_view_map_;

  void doCreateKeyframeView(QSharedPointer<effect::Keyframe<T>> keyframe) {
    KeyframeView<T>* kf_view = new KeyframeView<T>(this, keyframe, theme_service_);
    Q_ASSERT(keyframe_view_map_.count(keyframe) == 0);
    keyframe_view_map_[keyframe] = kf_view;
    updateKeyframeViewPosition(kf_view);
  }

  void updateAll() {
    for (auto const& kfp : keyframe_view_map_) {
      auto kfv = kfp.second;
      updateKeyframeViewPosition(kfv);
    }
  }

  void updateKeyframeViewPosition(KeyframeView<T>* keyframe_view) {
    auto keyframe = keyframe_view->keyframe();
    int st = sequence_scroll_view_->start_time();
    int ed = sequence_scroll_view_->end_time();
    int64_t trt = keyframe->time() - clip_->b_time() + clip_->start_time();
    if (st <= trt && trt <= ed) {
      int x = sequence_scroll_view_->getPositionRelativeToView(trt);
      keyframe_view->setGeometry(x - 6, 0, 13, 13);
      keyframe_view->show();
    }
    else {
      keyframe_view->hide();
    }
  }

public:
  PropertyTimelineView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout_params,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Property<T>> property,
    SequenceScrollView* sequence_scroll_view,  
    QSharedPointer<IThemeService> theme_service) :
  Div(parent), theme_service_(theme_service), layout_params_(layout_params), sequence_(sequence),
  property_(property), clip_(clip), sequence_scroll_view_(sequence_scroll_view) {
  
  property->onDidUpdate.connect(
    sig2_t<void ()>::slot_type([this]() {
      update();
    }).track(__sig_scope_));

  for (auto kfp : property_->keyframes()) {
    auto kf = kfp.second;
    doCreateKeyframeView(kf);
  }
  property->onDidAddKeyframe.connect(SIG2_TRACK(sig2_t<void (QSharedPointer<effect::Keyframe<T>>)>::slot_type(
    [this](QSharedPointer<effect::Keyframe<T>> keyframe) {
    doCreateKeyframeView(keyframe);
  })));
  clip->onDidUpdate.connect(SIG2_TRACK(sig2_t<void()>::slot_type(
    [this]() {
      updateAll();
  })));

  setBorder(Div::BOTTOM, 1, theme_service->getTheme().surfaceBrightColor());
  theme_service->onDidUpdate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
    [this]() {
      setBorder(Div::BOTTOM, 1, theme_service_->getTheme().surfaceBrightColor());
  })));
}  

};

}

}

#endif