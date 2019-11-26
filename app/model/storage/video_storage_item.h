#ifndef NLIVE_VIDEO_STORAGE_ITEM_H_
#define NLIVE_VIDEO_STORAGE_ITEM_H_

#include "base/common/memory.h"

#include "model/resource/video_resource.h"
#include "model/resource/audio_resource.h"
#include "model/storage/storage_item.h"

namespace nlive {

class VideoStorageItem : public StorageItem {

private:
  VideoStorageItem() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<StorageItem>(*this);
    ar & video_resource_;
    ar & audio_resource_;
  }

  sptr<VideoResource> video_resource_;
  sptr<AudioResource> audio_resource_;

public:
  static const std::string TYPE;

  VideoStorageItem(
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