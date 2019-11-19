#ifndef NLIVE_WIDGETS_TASK_BAR_H_
#define NLIVE_WIDGETS_TASK_BAR_H_

#include <QWidget>
#include <QSharedPointer>
#include "platform/task/task_service.h"

namespace nlive {

namespace task_bar {

class TaskBar : public QWidget {

public:
  TaskBar(QWidget* parent, QSharedPointer<ITaskService> task_service);

};

}

}

#endif