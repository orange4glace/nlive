#include "model/storage/video_resource_storage_item.h"

#include "model/sequence/clip.h"

namespace nlive {

const std::string VideoResourceStorageItem::TYPE = "nlive.StorageItem.VideoResourceStorageItem";

VideoResourceStorageItem::VideoResourceStorageItem(
  QSharedPointer<StorageItem> parent,
  QSharedPointer<VideoResource> video_resource) :
  ResourceStorageItem(VideoResourceStorageItem::TYPE, parent, video_resource),
  video_resource_(video_resource) {

}

QSharedPointer<Clip> VideoResourceStorageItem::cliperize(Rational time_base) {
  qDebug() << "cliperize " << video_resource_->duration() << " " <<
      video_resource_->time_base().num() << video_resource_->time_base().den() << " " <<
      time_base.num() << " " << time_base.den() << "\n";
  return QSharedPointer<Clip>(new Clip(nullptr, time_base, 0,
    Rational::rescale(video_resource_->duration(), video_resource_->time_base(), time_base), 0));
}

QSharedPointer<VideoResource> VideoResourceStorageItem::video_resource() {
  return video_resource_;
}

}