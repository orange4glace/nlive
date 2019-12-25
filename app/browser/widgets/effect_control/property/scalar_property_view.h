#ifndef NLIVE_EFFECT_CONTROL_SCALAR_PROPERTY_VIEW_H_
#define NLIVE_EFFECT_CONTROL_SCALAR_PROPERTY_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include "model/effect/property.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/value/scalar.h"
#include "browser/widgets/effect_control/property/property_view.h"
#include "browser/widgets/effect_control/property/number_input_box.h"

namespace nlive {

namespace effect_control {

class ScalarPropertyView : public PropertyViewTpl<effect::value::Scalar> {

private:
  NumberInputBox* input_box_;

protected:
  void updateValue() override;

public:
  ScalarPropertyView(
    QWidget* parent,
    sptr<EffectControlLayout> layout,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Property<effect::value::Scalar>> property,
    QString label,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service);

  inline NumberInputBox* input_box() { return input_box_; }

};

}

}

#endif