#ifndef NLIVE_TASK_SERVICE_H_
#define NLIVE_TASK_SERVICE_H_

#include <functional>
#include <QObject>
#include "base/common/memory.h"
#include "base/common/sig.h"

namespace nlive {

class Task;

class ITaskService : public QObject {
  Q_OBJECT

protected:
  inline ITaskService() {}

public:
  virtual void queueTask(sptr<Task> task, std::function<void(sptr<Task>)>&& callback) = 0;
  virtual std::vector<sptr<Task>>& tasks() = 0;

  sig2_t<void (sptr<Task>)> onDidAddTask;
  sig2_t<void (sptr<Task>)> onWillRemoveTask;
  sig2_t<void (sptr<Task>)> onDidRemoveTask;

};

}

#endif