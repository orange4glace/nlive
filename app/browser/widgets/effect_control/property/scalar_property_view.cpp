#include "browser/widgets/effect_control/property/scalar_property_view.h"

#include <QDebug>

namespace nlive {

namespace effect_control {

ScalarPropertyView::ScalarPropertyView(
  QWidget* parent,
  QSharedPointer<EffectControlLayout> layout_params,
  QSharedPointer<Sequence> sequence,
  QSharedPointer<Clip> clip,
  QSharedPointer<effect::Property<effect::value::Scalar>> property,
  QString label,
  SequenceScrollView* sequence_scroll_view,
  QSharedPointer<IThemeService> theme_service) :
  PropertyView<effect::value::Scalar>(
    parent, layout_params, sequence, clip, property, label, sequence_scroll_view, theme_service) {
  auto form_view = this->form_view();
  input_box_ = new NumberInputBox(nullptr, 0);
  input_box_->setTextColor(theme_service->getTheme().surfaceTextColor());
  theme_service->onDidUpdate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type([this]() {
    input_box_->setTextColor(theme_service_->getTheme().surfaceTextColor());
  })));
  form_view->addInputView(input_box_);
  updateValue();

  input_box_->onDidChangeValue.connect(
    sig2_t<void (double)>::slot_type
    ([this, sequence, clip, property](double value) {
      int64_t timecode_offset = sequence->getClipBTimecodeOffset(clip);
      auto v = property_->getInterpolatedValue(timecode_offset);
      property->upsertKeyframe(timecode_offset, effect::value::Scalar(value));
    }).track(__sig_scope_));
}

void ScalarPropertyView::updateValue() {
  int64_t timecode_offset = sequence_->getClipBTimecodeOffset(clip_);
  auto value = property_->getInterpolatedValue(timecode_offset);
  input_box_->setValue(value.value(), false);
}

}

}