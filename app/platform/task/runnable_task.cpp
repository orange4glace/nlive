#include "platform/task/runnable_task.h"

#include <QMetaObject>
#include <QThread>
#include <QDebug>
#include "platform/task/task.h"

namespace nlive {

RunnableTask::RunnableTask(QSharedPointer<Task> task) : task_(task) {
  setAutoDelete(false);
}

void RunnableTask::run() {
  task_->start();
  QMetaObject::invokeMethod(task_.get(), "finished", Qt::QueuedConnection, Q_ARG(QSharedPointer<Task>, task_));
  QMetaObject::invokeMethod(this, "completed", Qt::QueuedConnection);
}

}