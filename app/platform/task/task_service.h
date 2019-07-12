#ifndef _NLIVE_TASK_SERVICE_H_
#define _NLIVE_TASK_SERVICE_H_

#include <functional>
#include <QObject>

namespace nlive {

class Task;

class ITaskService : public QObject {
  Q_OBJECT

protected:
  inline ITaskService() {}

public:
  virtual void queueTask(Task* task, std::function<void(Task*)>&& callback) = 0;

};

}

#endif