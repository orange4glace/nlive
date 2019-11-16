#ifndef NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_H_
#define NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_H_

#include <QSharedPointer>

#include "model/resource/video_resource.h"
#include "model/resource/audio_resource.h"
#include "model/storage/storage_item.h"

namespace nlive {

class VideoResourceStorageItem : public StorageItem {

private:
  QSharedPointer<VideoResource> video_resource_;
  QSharedPointer<AudioResource> audio_resource_;

public:
  static const std::string TYPE;

  VideoResourceStorageItem(
    QSharedPointer<StorageItem> parent,
    QString name,
    QSharedPointer<VideoResource> video_resource,
    QSharedPointer<AudioResource> audio_resouce);

  QSharedPointer<Clip> cliperize(QSharedPointer<Sequence> sequence) override;

  QSharedPointer<VideoResource> video_resource();
  QSharedPointer<AudioResource> audio_resource();

  inline bool is_directory() const { return false; }

};

}

#endif