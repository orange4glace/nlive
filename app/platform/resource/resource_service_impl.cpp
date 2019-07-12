#include "platform/resource/resource_service_impl.h"

#include "platform/task/task_service.h"
#include "platform/task/task.h"

#include <QDebug>
#include <QThread>

namespace nlive {

namespace {

class ResourceLoadTask : public Task {
private:
  QString path_;
  QSharedPointer<Resource> result_;
public:
  ResourceLoadTask(QString path) : path_(path), result_(nullptr) {
  }
  QSharedPointer<Resource> result() {
    return result_;
  }
protected:
  void run() {
  }
};

}

ResourceService::ResourceService(ITaskService* task_service) :
  task_service_(task_service) {

}

void ResourceService::loadResource(QString path, std::function<void(QSharedPointer<Resource>)>&& callback) {
  ResourceLoadTask* task = new ResourceLoadTask(path);
  task_service_->queueTask(task, [callback](Task* task) {
    ResourceLoadTask* rtask = static_cast<ResourceLoadTask*>(task);
    callback(rtask->result());
  });
}

}