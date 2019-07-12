#ifndef _NLIVE_TASK_SERVICE_IMPL_H_
#define _NLIVE_TASK_SERVICE_IMPL_H_

#include "platform/task/task_service.h"
#include <QThread>
#include <QRunnable>

namespace nlive {

class TaskService : public ITaskService {
  Q_OBJECT

private:
  QThread* thread_;

public:
  TaskService();

  void queueTask(Task* task, std::function<void(Task*)>&& callback) override;

};

}

#endif