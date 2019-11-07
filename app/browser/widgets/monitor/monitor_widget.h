#ifndef NLIVE_MONITOR_WIDGET_H_
#define NLIVE_MONITOR_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <vector>

#include "base/common/sig.h"

namespace nlive {

namespace timelinewidget {
  
class TimelineWidget;

}

class ITimelineWidgetService;
class IThemeService;
class Sequence;

namespace monitor_widget {

class SequenceView;

class MonitorWidget : public QDockWidget, public Sig {

private:
  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;
  QSharedPointer<IThemeService> theme_service_;

  std::vector<sig2_conn_t> widget_connections_;
  SequenceView* sequence_view_;

  void handleDidChangeCurrentTimelineWidget(timelinewidget::TimelineWidget* timeline_widget);
  void handleDidChangeSequence(QSharedPointer<Sequence> sequence);

protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

public:
  MonitorWidget(QWidget* parent,
    QSharedPointer<ITimelineWidgetService> timeline_widget_service,
    QSharedPointer<IThemeService> theme_service);

};

}

}

#endif