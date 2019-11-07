#ifndef NLIVE_RESOURCE_SERVICE_IMPL_H_
#define NLIVE_RESOURCE_SERVICE_IMPL_H_

#include "platform/resource/resource_service.h"

namespace nlive {

class ITaskService;

class ResourceService : public IResourceService {

private:
  ITaskService* task_service_;

public:
  ResourceService(ITaskService* task_service);

  void loadResource(QString path, std::function<void(QSharedPointer<Resource>)>&& callback) override;

};

}

#endif