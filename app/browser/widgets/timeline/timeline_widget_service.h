#ifndef NLIVE_TIMELINE_WIDGET_SERVICE_H_
#define NLIVE_TIMELINE_WIDGET_SERVICE_H_

#include "base/common/sig.h"
#include "browser/widgets/timeline/timelinewidget.h"

namespace nlive {

class ITimelineWidgetService : public Sig {

protected:
  inline ITimelineWidgetService() {}

public:
  virtual void setCurrentWidget(timelinewidget::TimelineWidget* widget) = 0;
  virtual timelinewidget::TimelineWidget* current_widget() = 0;

  sig2_t<void (timelinewidget::TimelineWidget* widget)> onDidChangeCurrentWidget;

};

}

#endif