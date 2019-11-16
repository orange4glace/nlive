#ifndef NLIVE_AUDIO_STORAGE_ITEM_H_
#define NLIVE_AUDIO_STORAGE_ITEM_H_

#include <QSharedPointer>

#include "model/resource/video_resource.h"
#include "model/resource/audio_resource.h"
#include "model/storage/storage_item.h"

namespace nlive {

class AudioStorageItem : public StorageItem {

private:
  QSharedPointer<AudioResource> audio_resource_;

public:
  static const std::string TYPE;

  AudioStorageItem(
    QSharedPointer<StorageItem> parent,
    QString name,
    QSharedPointer<AudioResource> audio_resource);

  QSharedPointer<Clip> cliperize(QSharedPointer<Sequence> sequence) override;

  QSharedPointer<AudioResource> audio_resource();

  inline bool is_directory() const { return false; }

};

}

#endif