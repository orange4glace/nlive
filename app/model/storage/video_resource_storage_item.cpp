#include "model/storage/video_resource_storage_item.h"

#include "model/sequence/clip.h"

namespace nlive {

VideoResourceStorageItem::VideoResourceStorageItem(
  QSharedPointer<StorageItem> parent,
  QSharedPointer<VideoResource> video_resource) :
  ResourceStorageItem("nlive.StorageItem.VideoResourceStorageItem", parent, video_resource),
  video_resource_(video_resource) {

}

QSharedPointer<Clip> VideoResourceStorageItem::cliperize(Timebase timebase) {
  return QSharedPointer<Clip>(new Clip(nullptr, timebase, 0,
    Timebase::rescale(video_resource_->duration(), video_resource_->timebase(), timebase), 0));
}

QSharedPointer<VideoResource> VideoResourceStorageItem::video_resource() {
  return video_resource_;
}

}