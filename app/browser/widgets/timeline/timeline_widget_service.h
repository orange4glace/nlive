#ifndef NLIVE_TIMELINE_WIDGET_SERVICE_H_
#define NLIVE_TIMELINE_WIDGET_SERVICE_H_

#include "base/common/sig.h"
#include "browser/widgets/timeline/timeline_widget.h"

namespace nlive {

class ITimelineWidgetService : public Sig {

protected:
  inline ITimelineWidgetService() {}

public:
  virtual void setCurrentWidget(timeline_widget::TimelineWidget* widget) = 0;
  virtual timeline_widget::TimelineWidget* current_widget() = 0;

  sig2_t<void (timeline_widget::TimelineWidget* widget)> onDidChangeCurrentWidget;

};

}

#endif