#ifndef NLIVE_TASK_H_
#define NLIVE_TASK_H_

#include <QObject>
#include <QSharedPointer>
#include <functional>
#include <mutex>
#include "base/common/sig.h"

namespace nlive {

class Task : public QObject, public Sig {
  Q_OBJECT

friend class TaskService;
friend class RunnableTask;

private:
  std::function<void(QSharedPointer<Task>)> callback_;
  qreal progress_;

private slots:
  inline void finished(QSharedPointer<Task> task) {
    Q_ASSERT(this == task.get());
    if (callback_ != nullptr) callback_(task);
  }

protected:
  virtual void run() = 0;

  inline void setProgress(qreal value) {
    progress_ = value;
    onUpdateProgress(value);
  }

public:
  inline Task() : 
    callback_(nullptr), progress_(0) {

  };

  inline void start() {
    run();
  }

  inline qreal progress() {
    return progress_;
  }

  sig2_t<void (qreal/*value*/)> onUpdateProgress;

};

}

#endif