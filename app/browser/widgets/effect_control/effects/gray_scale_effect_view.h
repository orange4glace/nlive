#ifndef NLIVE_EFFECT_CONTROL_GRAY_SCALE_EFFECT_VIEW_H_
#define NLIVE_EFFECT_CONTROL_GRAY_SCALE_EFFECT_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <vector>

#include "platform/logger/logger.h"
#include "model/effect/gray_scale_effect.h"
#include "browser/widgets/effect_control/effect_view.h"
#include "browser/widgets/effect_control/effect_view_factory.h"
#include "browser/widgets/effect_control/property/scalar_property_view.h"

namespace nlive {

namespace effect_control {

class GrayScaleEffectView : public EffectView {

private:
  ScalarPropertyView* scale_property_view_;

public:
  inline GrayScaleEffectView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout_params,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::GrayScaleEffect> effect,
    SequenceScrollView* sequence_scroll_view,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IMementoService> memento_service) :
    EffectView(parent, layout_params, sequence, clip, effect, sequence_scroll_view, theme_service, memento_service) {
    scale_property_view_ =
        new ScalarPropertyView(this, layout_params, sequence, clip, effect->scale(),
          QString("Scale"), sequence_scroll_view, theme_service);
    scale_property_view_->input_box()->setSlidingStep(0.1);
    insertPropertyView(scale_property_view_, 0);
  }

};

class GrayScaleEffectViewFactory : public EffectViewFactory {

public:
  inline GrayScaleEffectViewFactory() {}
  inline GrayScaleEffectView* create(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout_params,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IMementoService> memento_service) override {
  if (effect->type() != effect::GrayScaleEffect::TYPE) {
    spdlog::get(LOGGER_DEFAULT)->warn(
      "[ScalarEffectViewFactory] Effect type not match! expected = {}, got = {}", effect::TransformEffect::TYPE, effect->type());
    return nullptr;
  }
  QSharedPointer<effect::GrayScaleEffect> gray_scale_effect = effect.staticCast<effect::GrayScaleEffect>();
  return new GrayScaleEffectView(parent, layout_params, sequence,
    clip, gray_scale_effect, sequence_scroll_view, theme_service, memento_service);
  }

};

}

}

#endif