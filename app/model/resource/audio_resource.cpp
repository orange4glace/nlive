#include "model/resource/audio_resource.h"

#include <QSharedPointer>

#include "platform/logger/logger.h"
#include "platform/task/task_service.h"
#include "platform/task/task.h"

namespace nlive {

const std::string AudioResource::TYPE = "nlive.Resource.AudioResource";

class AudioResourceRawConvertingTask : public Task {
private:
  QSharedPointer<AudioResource> resource_;
protected:
  void run() {
    
  }
public:
  AudioResourceRawConvertingTask(QSharedPointer<AudioResource> resource) : 
    resource_(resource) {

  }
};

AudioResource::AudioResource(std::string path) :
  Resource(AudioResource::TYPE, path) {

}

int64_t AudioResource::sample_rate() const {
  return sample_rate_;
}

int64_t AudioResource::duration() const {
  return duration_;
}

}