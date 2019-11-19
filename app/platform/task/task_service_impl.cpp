#include "platform/task/task_service_impl.h"

#include <QDebug>
#include <QThreadPool>
#include "platform/task/task.h"
#include "platform/task/runnable_task.h"

namespace nlive {

TaskService::TaskService() {
}

void TaskService::queueTask(QSharedPointer<Task> task, std::function<void(QSharedPointer<Task>)>&& callback) {
  tasks_.push_back(task);
  onDidAddTask(task);
  task->callback_ = std::move(callback);
  RunnableTask* runnable_task = new RunnableTask(task);
  connect(runnable_task, &RunnableTask::completed, this, [this, runnable_task, task]() {
    auto taskie = runnable_task->task();
    onWillRemoveTask(task);
    tasks_.erase(std::remove(tasks_.begin(), tasks_.end(), task), tasks_.end());
    runnable_task->deleteLater();
  }, Qt::QueuedConnection);
  QThreadPool::globalInstance()->start(runnable_task);
}

std::vector<QSharedPointer<Task>>& TaskService::tasks() {
  return tasks_;
}

}