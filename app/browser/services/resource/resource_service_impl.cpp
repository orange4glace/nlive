#include "browser/services/resource/resource_service_impl.h"

#include "platform/task/task_service.h"

namespace nlive {

namespace {
  struct work {
    QString path;
    Resource* result;
    void (*callback)(Resource* resource);
  };

  void doLoadResource(uv_work_t* req) {
    work* w = (work*)(req->data);
    QString& path = w->path;
  }

  void doLoadResourceCallback(uv_work_t* req, int status) {
    work* w = (work*)(req->data);
    Resource* result = w->result;
    w->callback(result);
    delete req;
  }
}

ResourceService::ResourceService(QSharedPointer<ITaskService> task_service) :
  task_service_(task_service) {

}

void ResourceService::loadResource(QString path, void (*callback)(Resource* resource)) {
  uv_work_t* req = new uv_work_t();
  auto w = new work();
  w->path = path;
  w->result = nullptr;
  w->callback = callback;
  req->data = w;
  task_service_->queueTask(doLoadResource, req, doLoadResourceCallback);
}

}