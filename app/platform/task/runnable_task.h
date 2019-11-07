#ifndef NLIVE_RUNNABLE_TASK_H_
#define NLIVE_RUNNABLE_TASK_H_

#include <QObject>
#include <QRunnable>

namespace nlive {

class Task;

class RunnableTask : public QRunnable {

private:
  Task* task_;

public:
  RunnableTask(Task* task);
  void run() override;

};

}

#endif