#ifndef NLIVE_TASK_SERVICE_IMPL_H_
#define NLIVE_TASK_SERVICE_IMPL_H_

#include "platform/task/task_service.h"
#include <QObject>
#include <QThread>
#include <QRunnable>
#include <vector>

namespace nlive {

class TaskService : public QObject, public ITaskService, public Sig {
  Q_OBJECT

private:
  std::vector<QSharedPointer<Task>> tasks_;

public:
  TaskService();

  void queueTask(QSharedPointer<Task> task, std::function<void(QSharedPointer<Task>)>&& callback) override;
  std::vector<QSharedPointer<Task>>& tasks() override;

};

}

#endif