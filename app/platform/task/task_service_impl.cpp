#include "platform/task/task_service_impl.h"

#include "platform/task/task.h"
#include "platform/task/runnable_task.h"

#include <QThreadPool>

namespace nlive {

TaskService::TaskService() {
  thread_ = new QThread();
  moveToThread(thread_);
  thread_->start();
}

void TaskService::queueTask(Task* task, std::function<void(Task*)>&& callback) {
  task->callback_ = std::move(callback);
  RunnableTask* runnable_task = new RunnableTask(task);
  QThreadPool::globalInstance()->start(runnable_task);
}

}