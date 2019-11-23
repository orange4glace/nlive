#ifndef NLIVE_EFFECT_CONTROL_CLIP_VIEW_H_
#define NLIVE_EFFECT_CONTROL_CLIP_VIEW_H_

#include <QWidget>
#include <QVBoxLayout>
#include "base/common/memory.h"
#include <vector>
#include <map>
#include "base/common/sig.h"
#include "browser/services/memento/memento_service.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

class IThemeService;
class Sequence;
class Clip;

namespace effect {

class Effect;

}

namespace effect_control {

class EffectControlLayout;
class EffectView;

class ClipView : public QWidget, public Sig {
  Q_OBJECT

private:
  sptr<IThemeService> theme_service_;
  sptr<IMementoService> memento_service_;
  sptr<EffectControlLayout> layout_params_;
  sptr<Sequence> sequence_;
  sptr<Clip> clip_;
  SequenceScrollView* sequence_scroll_view_;

  std::vector<std::pair<sptr<effect::Effect>, EffectView*>> effect_views_;
  std::map<sptr<effect::Effect>, EffectView*> effect_view_map_;

  void addEffectView(sptr<effect::Effect> effect, int index);

  void doLayout();

protected:
  bool event(QEvent* event) override;

public:
  ClipView(
    QWidget* parent,
    sptr<EffectControlLayout> layout,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service,
    sptr<IMementoService> memento_service);

  QSize sizeHint() const;

};

}

}

#endif