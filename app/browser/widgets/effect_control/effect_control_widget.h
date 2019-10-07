#ifndef NLIVE_EFFECT_CONTROL_WIDGET_H_
#define NLIVE_EFFECT_CONTROL_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <QMetaObject>

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

class EffectControlWidget : public QDockWidget {
  Q_OBJECT

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;
  timelinewidget::TimelineWidget* target_timeline_widget_;
  QMetaObject::Connection timeline_widget_connection_;
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
    QSharedPointer<ITimelineWidgetService> timeline_widget_service);

};

}

}

#endif