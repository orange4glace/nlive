#ifndef NLIVE_RUNNABLE_TASK_H_
#define NLIVE_RUNNABLE_TASK_H_

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include "base/common/memory.h"

namespace nlive {

class Task;

class RunnableTask : public QObject, public QRunnable {
  Q_OBJECT

private:
  sptr<Task> task_;

public:
  RunnableTask(sptr<Task> task);
  void run() override;

  inline sptr<Task> task() {
    return task_;
  }

signals:
  void completed();

};

}

#endif