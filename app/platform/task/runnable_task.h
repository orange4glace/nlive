#ifndef NLIVE_RUNNABLE_TASK_H_
#define NLIVE_RUNNABLE_TASK_H_

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QSharedPointer>

namespace nlive {

class Task;

class RunnableTask : public QObject, public QRunnable {
  Q_OBJECT

private:
  QSharedPointer<Task> task_;

public:
  RunnableTask(QSharedPointer<Task> task);
  void run() override;

  inline QSharedPointer<Task> task() {
    return task_;
  }

signals:
  void completed();

};

}

#endif