#ifndef NLIVE_EFFECT_CONTROL_WIDGET_H_
#define NLIVE_EFFECT_CONTROL_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <QMetaObject>
#include "base/common/sig.h"

#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "browser/services/memento/memento_service.h"

namespace nlive {

class IThemeService;
class ITimelineWidgetService;

namespace timelinewidget {

class TimelineWidget;
class SequenceView;

}

namespace effect_control {

class EffectControlLayout;
class SequenceView;

class EffectControlWidget : public QDockWidget, public Sig {
  Q_OBJECT

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<IMementoService> memento_service_;

  SequenceScrollView* sequence_scroll_view_;

  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;
  timelinewidget::TimelineWidget* target_timeline_widget_;
  sig2_conn_t timeline_widget_connection_;
  effect_control::SequenceView* sequence_view_;

  QSharedPointer<EffectControlLayout> layout_params_;

  void setTargetTimelineWidget(timelinewidget::TimelineWidget* widget);
  void setTargetTimelineWidgetSequenceView(timelinewidget::SequenceView* sequence_view);
  
protected:
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

public:
  static void Initialize();

  EffectControlWidget(
    QWidget* parent,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<ITimelineWidgetService> timeline_widget_service,
    QSharedPointer<IMementoService> memento_service);

};

}

}

#endif