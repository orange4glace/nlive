#ifndef NLIVE_TASK_H_
#define NLIVE_TASK_H_

#include <QObject>
#include <functional>
#include <mutex>

namespace nlive {

class Task : public QObject {
  Q_OBJECT

friend class TaskService;
friend class RunnableTask;

private:
  std::function<void(QSharedPointer<Task>)> callback_;
  std::mutex mutex_;
  qreal progress_;

private slots:
  inline void finished(QSharedPointer<Task> task) {
    Q_ASSERT(this == task);
    if (callback_ != nullptr) callback_(task);
  }

protected:
  virtual void run() = 0;

  inline void setProgress(qreal value) {
    mutex_.lock();
    progress_ = value;
    mutex_.unlock();
    emit onUpdateProgress(value);
  }

public:
  inline Task() : 
    callback_(nullptr), progress_(0) {

  };

  inline void start() {
    run();
  }

  inline qreal progress() {
    mutex_.lock();
    auto val = progress_;
    mutex_.unlock();
    return val; 
  }

signals:
  void onUpdateProgress(qreal value);

};

}

#endif