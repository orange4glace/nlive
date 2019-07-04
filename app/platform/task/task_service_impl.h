#ifndef _NLIVE_TASK_SERVICE_IMPL_H_
#define _NLIVE_TASK_SERVICE_IMPL_H_

#include "platform/task/task_service.h"

namespace nlive {

class TaskService : public ITaskService {
  Q_OBJECT

private:
  uv_loop_t* loop_;

public:
  TaskService();

  void queueTask(uv_work_cb work_cb, uv_work_t* req, uv_after_work_cb after_work_cb) override;

};

}

#endif