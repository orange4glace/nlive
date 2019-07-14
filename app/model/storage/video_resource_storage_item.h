#ifndef _NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_H_
#define _NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_H_

#include <QSharedPointer>

#include "model/resource/video_resource.h"
#include "model/storage/resource_storage_item.h"

namespace nlive {

class VideoResourceStorageItem : public ResourceStorageItem {

private:
  QSharedPointer<VideoResource> video_resource_;

public:
  static const std::string TYPE;

  VideoResourceStorageItem(
    QSharedPointer<StorageItem> parent,
    QSharedPointer<VideoResource> video_resource);

  QSharedPointer<Clip> cliperize(Rational time_base) override;

  QSharedPointer<VideoResource> video_resource();

  inline bool is_directory() const { return false; }

};

}

#endif