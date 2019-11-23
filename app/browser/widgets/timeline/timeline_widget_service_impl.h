#ifndef NLIVE_TIMELINE_WIDGET_SERVICE_IMPL_H_
#define NLIVE_TIMELINE_WIDGET_SERVICE_IMPL_H_

#include "base/common/memory.h"

#include "browser/widgets/timeline/timeline_widget_service.h"
#include "browser/widgets/timeline/timeline_widget.h"

namespace nlive {

class TimelineWidgetService : public ITimelineWidgetService {

private:
  static sptr<TimelineWidgetService> instance_;

  timeline_widget::TimelineWidget* current_widget_;

public:
  static void Initialize();
  inline static sptr<TimelineWidgetService> const instance() {
    return TimelineWidgetService::instance_;
  }

  TimelineWidgetService();

  void setCurrentWidget(timeline_widget::TimelineWidget* widget) override;
  timeline_widget::TimelineWidget* current_widget() override;

};

}

#endif