#ifndef NLIVE_EFFECT_CONTROL_PROPERTY_TIMELINE_VIEW_H_
#define NLIVE_EFFECT_CONTROL_PROPERTY_TIMELINE_VIEW_H_

#include <QWidget>
#include <QPainter>
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/property.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

namespace effect_control {

template <class T>
class PropertyTimelineView : public QWidget, public Sig {

private:
  QSharedPointer<EffectControlLayout> layout_params_;
  QSharedPointer<Sequence> sequence_;
  QSharedPointer<Clip> clip_;
  QSharedPointer<effect::Property<T>> property_;
  SequenceScrollView* sequence_scroll_view_;

protected:
  void paintEvent(QPaintEvent* e) override {
    QPainter p(this);
    p.fillRect(rect(), Qt::lightGray);
    int64_t timeoffset = sequence_->getClipBTimecodeOffset(clip_);
    auto keyframes = property_->keyframes();
    int st = sequence_scroll_view_->start_time();
    int ed = sequence_scroll_view_->end_time();
    for (auto pkf : keyframes) {
      auto& kf = pkf.second;
      int64_t trt = kf.time() - clip_->b_time() + clip_->start_time();
      if (st <= trt && trt <= ed) {
        int x = sequence_scroll_view_->getPositionRelativeToView(trt);
        p.fillRect(x, 0, 10, 10, Qt::red);
      }
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
  QWidget(parent), layout_params_(layout_params), sequence_(sequence),
  property_(property), clip_(clip), sequence_scroll_view_(sequence_scroll_view) {
  
  property->onDidUpdate.connect(
    sig2_t<void ()>::slot_type([this]() {
      update();
    }).track(__sig_scope_));
}  

};

}

}

#endif