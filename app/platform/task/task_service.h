#ifndef NLIVE_TASK_SERVICE_H_
#define NLIVE_TASK_SERVICE_H_

#include <functional>
#include <QObject>
#include <QSharedPointer>
#include "base/common/sig.h"

namespace nlive {

class Task;

class ITaskService : public QObject {
  Q_OBJECT

protected:
  inline ITaskService() {}

public:
  virtual void queueTask(QSharedPointer<Task> task, std::function<void(QSharedPointer<Task>)>&& callback) = 0;
  virtual std::vector<QSharedPointer<Task>>& tasks() = 0;

  sig2_t<void (QSharedPointer<Task>)> onDidAddTask;
  sig2_t<void (QSharedPointer<Task>)> onWillRemoveTask;
  sig2_t<void (QSharedPointer<Task>)> onDidRemoveTask;

};

}

#endif