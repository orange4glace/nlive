#include "model/storage/audio_storage_item.h"

#include "model/sequence/clip.h"
#include "model/sequence/audio_clip.h"

namespace nlive {

const std::string AudioStorageItem::TYPE = "nlive.StorageItem.AudioStorageItem";

AudioStorageItem::AudioStorageItem(
  QSharedPointer<StorageItem> parent,
  QString name,
  QSharedPointer<AudioResource> audio_resource) :
  StorageItem(AudioStorageItem::TYPE, name, parent),
  audio_resource_(audio_resource) {

}

QSharedPointer<Clip> AudioStorageItem::cliperize(Rational time_base) {
  qDebug() << "cliperize " << audio_resource_->duration() << " " <<
      audio_resource_->time_base().num() << audio_resource_->time_base().den() << " " <<
      time_base.num() << " " << time_base.den() << "\n";
  return QSharedPointer<Clip>(new AudioClip(nullptr, audio_resource_, time_base,
    0, Rational::rescale(audio_resource_->duration(), audio_resource_->time_base(), time_base), 0));
}

QSharedPointer<AudioResource> AudioStorageItem::audio_resource() {
  return audio_resource_;
}

}