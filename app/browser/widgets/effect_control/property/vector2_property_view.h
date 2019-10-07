#ifndef NLIVE_EFFECT_CONTROL_VECTOR2_PROPERTY_VIEW_H_
#define NLIVE_EFFECT_CONTROL_VECTOR2_PROPERTY_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include "model/effect/property.h"
#include "model/effect/value/vector2.h"
#include "browser/widgets/effect_control/property/property_view.h"

namespace nlive {

class Sequence;
class Clip;

namespace effect_control {

class EffectControlLayout;
class NumberInputBox;

class Vector2PropertyView : public PropertyView<effect::Property<effect::value::Vector2>> {

private:
  NumberInputBox* x_input_box_;
  NumberInputBox* y_input_box_;

protected:
  void updateValue() override;

public:
  Vector2PropertyView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Property<effect::value::Vector2>> property,
    QString label,
    QSharedPointer<IThemeService> theme_service);

};

}

}

#endif