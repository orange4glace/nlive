#include "browser/widgets/timeline/timeline_widget_service_impl.h"

namespace nlive {

QSharedPointer<TimelineWidgetService> TimelineWidgetService::instance_ = nullptr;

void TimelineWidgetService::Initialize() {
  TimelineWidgetService::instance_ = 
    QSharedPointer<TimelineWidgetService>(new TimelineWidgetService());
}

TimelineWidgetService::TimelineWidgetService() :
  current_widget_(nullptr) {

}

void TimelineWidgetService::setCurrentWidget(timelinewidget::TimelineWidget* widget) {
  if (current_widget_ == widget) return;
  current_widget_ = widget;
  onDidChangeCurrentWidget(widget);
}

timelinewidget::TimelineWidget* TimelineWidgetService::current_widget() {
  return current_widget_;
}

}