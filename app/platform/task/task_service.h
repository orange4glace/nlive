#ifndef _NLIVE_TASK_SERVICE_H_
#define _NLIVE_TASK_SERVICE_H_

#include <QObject>

#include <uv.h>

namespace nlive {

class ITaskService : public QObject {
  Q_OBJECT

protected:
  inline ITaskService() {}

public:
  // TODO : virtualize (without uvlib)
  virtual void queueTask(uv_work_cb work_cb, uv_work_t* req, uv_after_work_cb after_work_cb) = 0;

};

}

#endif