#ifndef NLIVE_SEQUENCE_STORAGE_ITEM_H_
#define NLIVE_SEQUENCE_STORAGE_ITEM_H_

#include "model/storage/storage_item.h"
#include "model/sequence/sequence.h"

namespace nlive {

class SequenceStorageItem : public StorageItem {

private:
  sptr<Sequence> sequence_;

public:
  static const std::string TYPE;

  SequenceStorageItem(
    sptr<StorageItem> parent,
    sptr<Sequence> sequence);

  sptr<Clip> cliperize(sptr<Sequence> sequence) override;

  inline sptr<Sequence> sequence() { return sequence_; }

  inline bool is_directory() const { return false; }

};

}

#endif