#ifndef NLIVE_EFFECT_CONTROL_WIDGET_H_
#define NLIVE_EFFECT_CONTROL_WIDGET_H_

#include <QDockWidget>
#include <QMetaObject>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "browser/widgets/widget.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "browser/services/memento/memento_service.h"
#include "browser/widgets/effect_control/keyframes_controller.h"

namespace nlive {

class IThemeService;
class ITimelineWidgetService;

namespace timeline_widget {

class TimelineWidget;
class SequenceView;

}

namespace effect_control {

class EffectControlLayout;
class SequenceView;

class EffectControlWidget : public Widget {
  Q_OBJECT

  DECLARE_WIDGET("nlive.widget.EffectControl");

private:
  sptr<IThemeService> theme_service_;
  sptr<IMementoService> memento_service_;

  sptr<ITimelineWidgetService> timeline_widget_service_;
  timeline_widget::TimelineWidget* target_timeline_widget_;
  sig2_conn_t timeline_widget_connection_;
  effect_control::SequenceView* sequence_view_;

  sptr<IKeyframesController> keyframes_controller_;
  sptr<EffectControlLayout> layout_params_;

  void setTargetTimelineWidget(timeline_widget::TimelineWidget* widget);
  void setTargetTimelineWidgetSequenceView(timeline_widget::SequenceView* sequence_view);
  
protected:
  void resizeEvent(QResizeEvent* event);

public:
  static void Initialize();

  EffectControlWidget(
    QWidget* parent,
    sptr<IThemeService> theme_service,
    sptr<ITimelineWidgetService> timeline_widget_service,
    sptr<IMementoService> memento_service,
    sptr<IWidgetsService> widgets_service);
  
  effect_control::SequenceView* sequence_view();
  sptr<IKeyframesController> keyframes_controller();

};

}

}

#endif