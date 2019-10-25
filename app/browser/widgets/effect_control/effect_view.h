#ifndef NLIVE_EFFECT_CONTROL_EFFECT_VIEW_H_
#define NLIVE_EFFECT_CONTROL_EFFECT_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <vector>

#include "platform/theme/themeservice.h"
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

class Clip;
class Sequence;

namespace effect {

class Effect;

}

namespace effect_control {

class EffectView : public QWidget {
  Q_OBJECT

private:
  std::vector<QWidget*> property_views_;

  void doLayout();
  void doPaint();

protected:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<EffectControlLayout> layout_;
  QSharedPointer<Clip> clip_;
  SequenceScrollView* sequence_scroll_view_;

  bool event(QEvent* event) override;

public:
  EffectView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    QSharedPointer<IThemeService> theme_service);

  void insertPropertyView(QWidget* view, int index);

  QSize sizeHint() const override;

};

}

}

#endif