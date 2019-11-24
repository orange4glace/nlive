#include "model/storage/sequence_storage_item.h"

#include <QDebug>

namespace nlive {

const std::string SequenceStorageItem::TYPE = "nlive.model.storage_item.SequenceStorageItem";

SequenceStorageItem::SequenceStorageItem(
  sptr<StorageItem> parent,
  QString name,
  sptr<Sequence> sequence) :
  StorageItem(parent->project(), SequenceStorageItem::TYPE, name, parent), sequence_(sequence) {
}

sptr<Clip> SequenceStorageItem::cliperize(sptr<Sequence> sequence) {
  // TODO : implement
  return nullptr;
}

}