#ifndef NLIVE_RESOURCE_SERVICE_IMPL_H_
#define NLIVE_RESOURCE_SERVICE_IMPL_H_

#include <map>
#include "platform/task/task_service.h"
#include "platform/resource/resource_service.h"

namespace nlive {



class AudioResourceRawConvertingService {

private:
  QSharedPointer<ITaskService> task_service_;
  std::map<QString, std::string> m_;

public:
  AudioResourceRawConvertingService(QSharedPointer<ITaskService> task_service);
  void process(QSharedPointer<AudioResource> resource);

};



class ResourceService : public IResourceService {

private:
  QSharedPointer<ITaskService> task_service_;
  AudioResourceRawConvertingService audio_resource_raw_converting_service_;

  std::map<QString, QSharedPointer<Resource>> resources_;

public:
  ResourceService(QSharedPointer<ITaskService> task_service);

  QSharedPointer<VideoResource> loadBestVideoResource(QString path) override;
  QSharedPointer<AudioResource> loadBestAudioResource(QString path) override;

};

}

#endif