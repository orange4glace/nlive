#include "browser/widgets/effect_control/transform/transform_effect_view.h"

#include <QDebug>

#include "platform/logger/logger.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/transform_effect.h"
#include "browser/widgets/effect_control/property/vector2_property_view.h"

namespace nlive {

namespace effect_control {

TransformEffectView::TransformEffectView(
  QWidget* parent,
  sptr<IKeyframesController> kfs_ctrl,
  sptr<EffectControlLayout> layout_params,
  sptr<Sequence> sequence,
  sptr<Clip> clip,
  sptr<effect::TransformEffect> effect,
  SequenceScrollView* sequence_scroll_view,
  sptr<IThemeService> theme_service,
  sptr<IMementoService> memento_service) :
  EffectView(parent, kfs_ctrl, layout_params, sequence, clip,
      effect, sequence_scroll_view, theme_service, memento_service) {
  position_property_view_ =
      new Vector2PropertyView(this, kfs_ctrl, layout_params, sequence, clip, effect->position(),
          QString("Position"), sequence_scroll_view,theme_service);
  scale_property_view_ =
      new Vector2PropertyView(this, kfs_ctrl, layout_params, sequence, clip, effect->scale(),
        QString("Scale"), sequence_scroll_view, theme_service);
  insertPropertyView(position_property_view_, 0);
  insertPropertyView(scale_property_view_, 1);
}

TransformEffectViewFactory::TransformEffectViewFactory() {}
TransformEffectView* TransformEffectViewFactory::create(
    QWidget* parent,
    sptr<IKeyframesController> keyframes_controller,
    sptr<EffectControlLayout> layout_params,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service,
    sptr<IMementoService> memento_service) {
  if (effect->type() != effect::TransformEffect::TYPE) {
    spdlog::get(LOGGER_DEFAULT)->warn(
      "[TransformEffectViewFactory] Effect type not match! expected = {}, got = {}", effect::TransformEffect::TYPE, effect->type());
    return nullptr;
  }
  sptr<effect::TransformEffect> transform_effect = std::static_pointer_cast<effect::TransformEffect>(effect);
  return new TransformEffectView(parent, keyframes_controller, layout_params, sequence,
    clip, transform_effect, sequence_scroll_view, theme_service, memento_service);
}


}

}