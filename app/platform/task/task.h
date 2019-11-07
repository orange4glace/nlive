#ifndef NLIVE_TASK_H_
#define NLIVE_TASK_H_

#include <functional>

#include <QObject>

namespace nlive {

class Task : public QObject {
  Q_OBJECT

friend class TaskService;
friend class RunnableTask;

public:
  inline Task() : callback_(nullptr) {};

  inline void start() {
    run();
  }

protected:
  virtual void run() = 0;

private:
  std::function<void(Task*)> callback_;

private slots:
  inline void finished(Task* task) {
    Q_ASSERT(this == task);
    if (callback_ != nullptr) callback_(task);
    // Delete self
    delete this;
  }

};

}

#endif