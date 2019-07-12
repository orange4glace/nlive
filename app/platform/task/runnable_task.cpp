#include "platform/task/runnable_task.h"

#include "platform/task/task.h"

#include <QMetaObject>

namespace nlive {

RunnableTask::RunnableTask(Task* task) : task_(task) {
}

void RunnableTask::run() {
  task_->start();
  QMetaObject::invokeMethod(task_, "finished", Qt::QueuedConnection, Q_ARG(Task*, task_));
}

}