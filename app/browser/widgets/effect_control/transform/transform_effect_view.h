#ifndef NLIVE_EFFECT_CONTROL_TRANSFORM_EFFECT_VIEW_H_
#define NLIVE_EFFECT_CONTROL_TRANSFORM_EFFECT_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include <vector>

#include "browser/widgets/effect_control/effect_view.h"
#include "browser/widgets/effect_control/effect_view_factory.h"

namespace nlive {

class Sequence;
class Clip;

namespace effect {

class Effect;
class TransformEffect;

namespace value {

class Vector2;

}

}

namespace effect_control {

class EffectControlLayout;
class Vector2PropertyView;

class TransformEffectView : public EffectView {
  Q_OBJECT

private:
  Vector2PropertyView* position_property_view_;
  Vector2PropertyView* scale_property_view_;

public:
  TransformEffectView(
    QWidget* parent,
    sptr<EffectControlLayout> layout,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::TransformEffect> effect,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service,
    sptr<IMementoService> memento_service);

};

class TransformEffectViewFactory : public EffectViewFactory {

public:
  TransformEffectViewFactory();
  TransformEffectView* create(
    QWidget* parent,
    sptr<EffectControlLayout> layout_params,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service,
    sptr<IMementoService> memento_service) override;

};

}

}

#endif