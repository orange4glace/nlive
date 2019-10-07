#ifndef _NLIVE_MONITOR_WIDGET_H_
#define _NLIVE_MONITOR_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <QMetaObject>
#include <vector>

namespace nlive {

namespace timelinewidget {
  
class TimelineWidget;

}

class ITimelineWidgetService;
class IThemeService;
class Sequence;

namespace monitor_widget {

class SequenceView;

class MonitorWidget : public QDockWidget {

private:
  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;
  IThemeService* theme_service_;

  std::vector<QMetaObject::Connection> widget_connections_;
  SequenceView* sequence_view_;

  void handleDidChangeCurrentTimelineWidget(timelinewidget::TimelineWidget* timeline_widget);
  void handleDidChangeSequence(QSharedPointer<Sequence> sequence);

protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

public:
  MonitorWidget(QWidget* parent,
    QSharedPointer<ITimelineWidgetService> timeline_widget_service,
    IThemeService* theme_service);

};

}

}

#endif