#ifndef NLIVE_TIMELINE_WIDGET_SERVICE_IMPL_H_
#define NLIVE_TIMELINE_WIDGET_SERVICE_IMPL_H_

#include <QSharedPointer>

#include "browser/widgets/timeline/timeline_widget_service.h"
#include "browser/widgets/timeline/timelinewidget.h"

namespace nlive {

class TimelineWidgetService : public ITimelineWidgetService {

private:
  static QSharedPointer<TimelineWidgetService> instance_;

  timelinewidget::TimelineWidget* current_widget_;

public:
  static void Initialize();
  inline static QSharedPointer<TimelineWidgetService> const instance() {
    return TimelineWidgetService::instance_;
  }

  TimelineWidgetService();

  void setCurrentWidget(timelinewidget::TimelineWidget* widget) override;
  timelinewidget::TimelineWidget* current_widget() override;

};

}

#endif