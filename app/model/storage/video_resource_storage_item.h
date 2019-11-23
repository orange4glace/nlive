#ifndef NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_H_
#define NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_H_

#include "base/common/memory.h"

#include "model/resource/video_resource.h"
#include "model/resource/audio_resource.h"
#include "model/storage/storage_item.h"

namespace nlive {

class VideoResourceStorageItem : public StorageItem {

private:
  sptr<VideoResource> video_resource_;
  sptr<AudioResource> audio_resource_;

public:
  static const std::string TYPE;

  VideoResourceStorageItem(
    sptr<StorageItem> parent,
    QString name,
    sptr<VideoResource> video_resource,
    sptr<AudioResource> audio_resouce);

  sptr<Clip> cliperize(sptr<Sequence> sequence) override;

  sptr<VideoResource> video_resource();
  sptr<AudioResource> audio_resource();

  inline bool is_directory() const { return false; }

};

}

#endif