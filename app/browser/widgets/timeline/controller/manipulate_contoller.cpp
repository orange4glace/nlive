#include "browser/widgets/timeline/controller/manipulate_controller.h"

#include "browser/widgets/timeline/timelinewidget.h"

namespace nlive {

namespace timelinewidget {

ManipulateController::ManipulateController(TimelineWidget* widget) :
  widget_(widget) {
  
  widget->installEventFilter(this);

}

bool ManipulateController::eventFilter(QObject* obj, QEvent* event) {
  
}

}

}