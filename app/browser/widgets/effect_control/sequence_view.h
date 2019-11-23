#ifndef NLIVE_EFFECT_CONTROL_SEQUENCE_VIEW_H_
#define NLIVE_EFFECT_CONTROL_SEQUENCE_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include <vector>
#include "base/common/sig.h"
#include "base/ui/solid_box.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "browser/services/memento/memento_service.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

class Clip;

namespace timeline_widget {

class SequenceView;

}

namespace effect_control {

class ClipView;

class SequenceView : public QWidget, public Sig {
  Q_OBJECT

private:
  sptr<IThemeService> theme_service_;
  sptr<IMementoService> memento_service_;
  sptr<EffectControlLayout> layout_params_;
  timeline_widget::SequenceView* timeline_widget_sequence_view_;
  SequenceScrollView* sequence_scroll_view_;

  sptr<Clip> clip_;
  ClipView* clip_view_;

  SolidBox* form_background_;
  SolidBox* timeline_background_;

  void handleDidChangeFocusedClips();
  void createClipView(sptr<Clip> clip);
  void clearClipView();

protected:
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

public:
  SequenceView(
    QWidget* parent,
    sptr<EffectControlLayout> layout,
    timeline_widget::SequenceView* timeline_widget_sequence_view,
    sptr<IThemeService> theme_service,
    sptr<IMementoService> memento_service);

};

}

}

#endif