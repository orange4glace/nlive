#ifndef _NLIVE_TIMELINE_WIDGET_MANIPULATE_CONTROLLER_H_
#define _NLIVE_TIMELINE_WIDGET_MANIPULATE_CONTROLLER_H_

#include <QObject>

namespace nlive {

namespace timelinewidget {

class TimelineWidget;

class ManipulateController : public QObject {
  Q_OBJECT

private:
  TimelineWidget* widget_;

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

public:
  ManipulateController(TimelineWidget* widget);

};

}

}

#endif