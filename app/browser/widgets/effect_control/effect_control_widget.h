#ifndef NLIVE_EFFECT_CONTROL_WIDGET_H_
#define NLIVE_EFFECT_CONTROL_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <QMetaObject>
#include "browser/widgets/widget.h"
#include "base/common/sig.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "browser/services/memento/memento_service.h"

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

class EffectControlWidget : public QDockWidget, public Widget, public Sig {
  Q_OBJECT
  
public:
  static const std::string TYPE;

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<IMementoService> memento_service_;

  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;
  timeline_widget::TimelineWidget* target_timeline_widget_;
  sig2_conn_t timeline_widget_connection_;
  effect_control::SequenceView* sequence_view_;

  QSharedPointer<EffectControlLayout> layout_params_;

  void setTargetTimelineWidget(timeline_widget::TimelineWidget* widget);
  void setTargetTimelineWidgetSequenceView(timeline_widget::SequenceView* sequence_view);
  
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

  inline std::string name() const override { return TYPE; }

};

}

}

#endif