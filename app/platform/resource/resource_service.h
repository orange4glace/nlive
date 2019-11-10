#ifndef NLIVE_RESOURCE_SERVICE_H_
#define NLIVE_RESOURCE_SERVICE_H_

#include <QSharedPointer>
#include <functional>
#include <QObject>
#include "model/resource/resource.h"
#include "model/resource/video_resource.h"
#include "model/resource/audio_resource.h"

namespace nlive {

class IResourceService : public QObject {
  Q_OBJECT

protected:
  inline IResourceService() {}

public:
  virtual QSharedPointer<VideoResource> loadBestVideoResource(QString path) = 0;
  virtual QSharedPointer<AudioResource> loadBestAudioResource(QString path) = 0;

};

}

#endif