#ifndef NLIVE_EFFECT_CONTROL_EFFECT_VIEW_H_
#define NLIVE_EFFECT_CONTROL_EFFECT_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include <vector>
#include "base/common/sig.h"
#include "base/layout/flex_layout.h"
#include "base/ui/svg_button.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/effect.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "browser/services/memento/memento_service.h"
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

namespace effect_control {

namespace {

class EffectViewHeader : public FlexLayout {

private:
  sptr<IThemeService> theme_service_;
  sptr<EffectControlLayout> layout_;
  sptr<Clip> clip_;
  sptr<effect::Effect> effect_;
  SequenceScrollView* sequence_scroll_view_;

  SvgButton* arrow_button_;

  bool opened_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  EffectViewHeader(
    QWidget* parent,
    sptr<EffectControlLayout> layout,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Effect> effect,
    sptr<IThemeService> theme_service);

  void setOpened(bool value);

  QSize sizeHint() const override;

  sig2_t<void (bool)> onArrowButtonClicked;

};

}

class EffectView : public QWidget, public Sig {
  Q_OBJECT

private:
  EffectViewHeader* header_;
  std::vector<QWidget*> property_views_;
  bool opened_;

  void doLayout();
  void doPaint();

protected:
  sptr<IThemeService> theme_service_;
  sptr<IMementoService> memento_service_;
  sptr<EffectControlLayout> layout_;
  sptr<Clip> clip_;
  sptr<effect::Effect> effect_;
  SequenceScrollView* sequence_scroll_view_;

  bool event(QEvent* event) override;

public:
  EffectView(
    QWidget* parent,
    sptr<EffectControlLayout> layout,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service,
    sptr<IMementoService> memento_service);

  void insertPropertyView(QWidget* view, int index);

  void setOpened(bool value);

  QSize sizeHint() const override;

};

}

}

#endif