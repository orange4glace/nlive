#ifndef NLIVE_TASK_SERVICE_IMPL_H_
#define NLIVE_TASK_SERVICE_IMPL_H_

#include "platform/task/task_service.h"
#include <QThread>
#include <QRunnable>
#include <vector>

namespace nlive {

class TaskService : public ITaskService, public Sig {
  Q_OBJECT

private:
  std::vector<sptr<Task>> tasks_;

public:
  TaskService();

  void queueTask(sptr<Task> task, std::function<void(sptr<Task>)>&& callback) override;
  std::vector<sptr<Task>>& tasks() override;

};

}

#endif