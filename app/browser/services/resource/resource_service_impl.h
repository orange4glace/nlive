#ifndef _NLIVE_RESOURCE_SERVICE_IMPL_H_
#define _NLIVE_RESOURCE_SERVICE_IMPL_H_

#include <QSharedPointer>
#include "browser/services/resource/resource_service.h"

namespace nlive {

class ITaskService;

class ResourceService : public IResourceService {

private:
  QSharedPointer<ITaskService> task_service_;

public:
  ResourceService(QSharedPointer<ITaskService> task_service);

  void loadResource(QString path, void (*uv_after_work_cb)(Resource* resource)) override;

};

}

#endif