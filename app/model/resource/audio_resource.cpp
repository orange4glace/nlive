#include "model/resource/audio_resource.h"

#include <QSharedPointer>
#include <QDebug>
#include <QString>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include "platform/logger/logger.h"
#include "platform/task/task_service.h"
#include "platform/task/task.h"

namespace nlive {

const std::string AudioResource::TYPE = "nlive.Resource.AudioResource";

AudioResource::AudioResource(std::string path, Rational time_base, int sample_rate, int64_t duration) :
  Resource(AudioResource::TYPE, path,
    QString::fromStdString(boost::filesystem::path(path).filename().string())),
  time_base_(time_base), sample_rate_(sample_rate), duration_(duration),
  raw_(nullptr) {

}

void AudioResource::setRaw(QSharedPointer<RawAudioResource> raw) {
  raw_ = raw;
  qDebug() << "Raw audio set" << raw;
}

}