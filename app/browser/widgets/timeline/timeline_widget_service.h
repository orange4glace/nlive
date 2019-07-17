#ifndef _NLIVE_TIMELINE_WIDGET_SERVICE_H_
#define _NLIVE_TIMELINE_WIDGET_SERVICE_H_

#include <QObject>

#include "browser/widgets/timeline/timelinewidget.h"

namespace nlive {

class ITimelineWidgetService : public QObject {
  Q_OBJECT

protected:
  inline ITimelineWidgetService() {}

public:
  virtual void setCurrentWidget(timelinewidget::TimelineWidget* widget) = 0;
  virtual timelinewidget::TimelineWidget* current_widget() = 0;

signals:
  void onDidChangeCurrentWidget(timelinewidget::TimelineWidget* widget);

};

}

#endif