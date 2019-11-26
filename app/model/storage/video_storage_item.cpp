#include "model/storage/video_storage_item.h"

#include "model/sequence/clip.h"
#include "model/sequence/video_clip.h"

namespace nlive {

const std::string VideoStorageItem::TYPE = "nlive.StorageItem.VideoStorageItem";

VideoStorageItem::VideoStorageItem(
  sptr<StorageItem> parent,
  QString name,
  sptr<VideoResource> video_resource,
  sptr<AudioResource> audio_resource) :
  StorageItem(parent->project(), VideoStorageItem::TYPE, name, parent),
  video_resource_(video_resource), audio_resource_(audio_resource) {

}

sptr<Clip> VideoStorageItem::cliperize(sptr<Sequence> sequence) {
  qDebug() << "cliperize " << video_resource_->duration() << " " <<
      video_resource_->time_base().num() << video_resource_->time_base().den() << " " <<
      sequence->time_base().num() << " " << sequence->time_base().den() << "\n";
  return sptr<Clip>(new VideoClip(nullptr, video_resource_, sequence->time_base(), sequence->sample_rate(),
    0, Rational::rescale(video_resource_->duration(), video_resource_->time_base(), sequence->time_base()), 0));
}

sptr<VideoResource> VideoStorageItem::video_resource() {
  return video_resource_;
}

sptr<AudioResource> VideoStorageItem::audio_resource() {
  return audio_resource_;
}

}