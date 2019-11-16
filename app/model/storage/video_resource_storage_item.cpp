#include "model/storage/video_resource_storage_item.h"

#include "model/sequence/clip.h"
#include "model/sequence/video_clip.h"

namespace nlive {

const std::string VideoResourceStorageItem::TYPE = "nlive.StorageItem.VideoResourceStorageItem";

VideoResourceStorageItem::VideoResourceStorageItem(
  QSharedPointer<StorageItem> parent,
  QString name,
  QSharedPointer<VideoResource> video_resource,
  QSharedPointer<AudioResource> audio_resource) :
  StorageItem(VideoResourceStorageItem::TYPE, name, parent),
  video_resource_(video_resource), audio_resource_(audio_resource) {

}

QSharedPointer<Clip> VideoResourceStorageItem::cliperize(QSharedPointer<Sequence> sequence) {
  qDebug() << "cliperize " << video_resource_->duration() << " " <<
      video_resource_->time_base().num() << video_resource_->time_base().den() << " " <<
      sequence->time_base().num() << " " << sequence->time_base().den() << "\n";
  return QSharedPointer<Clip>(new VideoClip(nullptr, video_resource_, sequence->time_base(), sequence->sample_rate(),
    0, Rational::rescale(video_resource_->duration(), video_resource_->time_base(), sequence->time_base()), 0));
}

QSharedPointer<VideoResource> VideoResourceStorageItem::video_resource() {
  return video_resource_;
}

QSharedPointer<AudioResource> VideoResourceStorageItem::audio_resource() {
  return audio_resource_;
}

}