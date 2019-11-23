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
  sptr<ITaskService> task_service_;
  std::map<std::string, std::vector<sptr<AudioResource>>> processing_map_;
  std::map<std::string, sptr<RawAudioResource>> processed_map_;

public:
  AudioResourceRawConvertingService(sptr<ITaskService> task_service);
  void process(sptr<AudioResource> resource);

};



class ResourceService : public IResourceService {

private:
  sptr<ITaskService> task_service_;
  AudioResourceRawConvertingService audio_resource_raw_converting_service_;

  std::map<QString, sptr<Resource>> resources_;

public:
  ResourceService(sptr<ITaskService> task_service);

  sptr<VideoResource> loadBestVideoResource(QString path) override;
  sptr<AudioResource> loadBestAudioResource(QString path) override;

};

}

#endif