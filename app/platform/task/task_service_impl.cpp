#include "platform/task/task_service_impl.h"

namespace nlive {

TaskService::TaskService() {
  loop_ = uv_default_loop();
}

void TaskService::queueTask(uv_work_cb work_cb, uv_work_t* req, uv_after_work_cb after_work_cb) {
  uv_queue_work(loop_, req, work_cb, after_work_cb);
}

}