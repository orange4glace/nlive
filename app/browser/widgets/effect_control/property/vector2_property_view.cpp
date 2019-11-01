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
  SequenceScrollView* sequence_scroll_view,
  QSharedPointer<IThemeService> theme_service) :
  PropertyView<effect::value::Vector2>(
    parent, layout_params, sequence, clip, property, label, sequence_scroll_view, theme_service) {
  auto form_view = this->form_view();
  x_input_box_ = new NumberInputBox(nullptr, 0);
  y_input_box_ = new NumberInputBox(nullptr, 0);
  x_input_box_->setTextColor(theme_service->getTheme().surfaceTextColor());
  y_input_box_->setTextColor(theme_service->getTheme().surfaceTextColor());
  theme_service->onDidUpdate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type([this]() {
    x_input_box_->setTextColor(theme_service_->getTheme().surfaceTextColor());
    y_input_box_->setTextColor(theme_service_->getTheme().surfaceTextColor());
  })));
  form_view->addInputView(x_input_box_);
  form_view->addInputView(y_input_box_);
  updateValue();

  x_input_box_->onDidChangeValue.connect(
    sig2_t<void (double)>::slot_type
    ([this, sequence, clip, property](double value) {
      int64_t timecode_offset = sequence->getClipBTimecodeOffset(clip);
      auto v = property_->getInterpolatedValue(timecode_offset);
      property->upsertKeyframe(timecode_offset, effect::value::Vector2(value, v.y()));
    }).track(__sig_scope_));
  y_input_box_->onDidChangeValue.connect(
    sig2_t<void (double)>::slot_type
    ([this, sequence, clip, property](double value) {
      int64_t timecode_offset = sequence->getClipBTimecodeOffset(clip);
      auto v = property_->getInterpolatedValue(timecode_offset);
      property->upsertKeyframe(timecode_offset, effect::value::Vector2(v.x(), value));
    }).track(__sig_scope_));
}

void Vector2PropertyView::updateValue() {
  int64_t timecode_offset = sequence_->getClipBTimecodeOffset(clip_);
  auto value = property_->getInterpolatedValue(timecode_offset);
  x_input_box_->setValue(value.x(), false);
  y_input_box_->setValue(value.y(), false);
}

}

}