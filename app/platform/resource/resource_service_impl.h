#ifndef NLIVE_RESOURCE_SERVICE_IMPL_H_
#define NLIVE_RESOURCE_SERVICE_IMPL_H_

#include <map>
#include "platform/resource/resource_service.h"

namespace nlive {

class ITaskService;

class ResourceService : public IResourceService {

private:
  ITaskService* task_service_;

  std::map<QString, QSharedPointer<Resource>> resources_;

public:
  ResourceService(ITaskService* task_service);

  QSharedPointer<VideoResource> loadBestVideoResource(QString path) override;
  QSharedPointer<AudioResource> loadBestAudioResource(QString path) override;

};

}

#endif