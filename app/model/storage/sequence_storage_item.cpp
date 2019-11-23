#include "model/storage/sequence_storage_item.h"

namespace nlive {

const std::string SequenceStorageItem::TYPE = "nlive.StorageItem.SequenceStorageItem";

SequenceStorageItem::SequenceStorageItem(
  sptr<StorageItem> parent,
  sptr<Sequence> sequence) :
  StorageItem(parent->project(), SequenceStorageItem::TYPE, sequence->name(), parent), sequence_(sequence) {

}

sptr<Clip> SequenceStorageItem::cliperize(sptr<Sequence> sequence) {
  // TODO : implement
  return nullptr;
}

}