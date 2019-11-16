#ifndef NLIVE_RESOURCE_SERVICE_IMPL_H_
#define NLIVE_RESOURCE_SERVICE_IMPL_H_

#include <map>
#include <vector>
#include "platform/task/task_service.h"
#include "platform/resource/resource_service.h"
#include "model/resource/raw_audio_resource.h"
#include "model/resource/audio_resource.h"

namespace nlive {



class AudioResourceRawConvertingService {

private:
  QSharedPointer<ITaskService> task_service_;
  std::map<std::string, std::vector<QSharedPointer<AudioResource>>> processing_map_;
  std::map<std::string, QSharedPointer<RawAudioResource>> processed_map_;

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