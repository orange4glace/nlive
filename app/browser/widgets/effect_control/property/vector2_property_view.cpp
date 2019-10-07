#include "browser/widgets/effect_control/property/vector2_property_view.h"

#include <QDebug>

#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/value/vector2.h"
#include "browser/widgets/effect_control/property/number_input_box.h"

namespace nlive {

namespace effect_control {

Vector2PropertyView::Vector2PropertyView(
  QWidget* parent,
  QSharedPointer<EffectControlLayout> layout_params,
  QSharedPointer<Sequence> sequence,
  QSharedPointer<Clip> clip,
  QSharedPointer<effect::Property<effect::value::Vector2>> property,
  QString label,
  QSharedPointer<IThemeService> theme_service) :
  PropertyView<effect::Property<effect::value::Vector2>>(
    parent, layout_params, sequence, clip, property, label, theme_service) {
  x_input_box_ = new NumberInputBox(this, 0);
  y_input_box_ = new NumberInputBox(this, 0);
  addInputView(x_input_box_);
  addInputView(y_input_box_);
  updateValue();
}

void Vector2PropertyView::updateValue() {
  int64_t offset_time = sequence_->current_time();
  auto value = property_->getInterpolatedValue(offset_time);
  x_input_box_->setValue(value.x());
  y_input_box_->setValue(value.y());
}

}

}