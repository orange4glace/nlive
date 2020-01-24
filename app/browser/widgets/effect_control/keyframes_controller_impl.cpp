#include "browser/widgets/effect_control/keyframes_controller_impl.h"

#include "browser/widgets/effect_control/sequence_view.h"
#include "browser/widgets/effect_control/clip_view.h"
#include "browser/widgets/effect_control/effect_view.h"
#include "browser/widgets/effect_control/property/property_view.h"

namespace nlive {

namespace effect_control {

KeyframesController::KeyframesController(EffectControlWidget* widget) :
  widget_(widget) {

}

void KeyframesController::translate(int64_t time) {
  auto seq_view = widget_->sequence_view();
  if (!seq_view) return;
  auto clip_view = seq_view->clip_view();
  if (!clip_view) return;
  bool valid = true;
  auto& effect_views = clip_view->effect_views();
  for (auto& effect_view : effect_views) {
    auto& property_views = effect_view.second->property_views();
    for (auto& property_view : property_views) {
      auto property = property_view->property();
      auto timeline_view = property_view->timeline_view();
      auto& focused_kfs = timeline_view->focused_keyframes();
      for (auto& kf : focused_kfs) {
        int64_t dst_time = kf->time() + time;
        if (property->getKeyframeAt(dst_time)) {
          valid = false;
        }
      }
    }
  }
  if (!valid) return;
}

void KeyframesController::remove() {
  auto seq_view = widget_->sequence_view();
  if (!seq_view) return;
  auto clip_view = seq_view->clip_view();
  if (!clip_view) return;
  bool valid = true;
  auto& effect_views = clip_view->effect_views();
  for (auto& effect_view : effect_views) {
    auto& property_views = effect_view.second->property_views();
    for (auto& property_view : property_views) {
      auto property = property_view->property();
      auto timeline_view = property_view->timeline_view();
      auto& focused_kfs = timeline_view->focused_keyframes();
      std::vector<sptr<effect::IKeyframe>> rems;
      for (auto& kf : focused_kfs) {
        rems.push_back(kf);
      }
      for (auto& kf : rems) {
        property->removeKeyframe(kf);
      }
    }
  }
}

}

}