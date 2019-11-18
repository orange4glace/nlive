#include "model/storage/sequence_storage_item.h"

namespace nlive {

const std::string SequenceStorageItem::TYPE = "nlive.StorageItem.SequenceStorageItem";

SequenceStorageItem::SequenceStorageItem(
  QSharedPointer<StorageItem> parent,
  QSharedPointer<Sequence> sequence) :
  StorageItem(SequenceStorageItem::TYPE, sequence->name(), parent), sequence_(sequence) {

}

QSharedPointer<Clip> SequenceStorageItem::cliperize(QSharedPointer<Sequence> sequence) {
  // TODO : implement
  return nullptr;
}

}