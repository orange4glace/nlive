#ifndef NLIVE_SEQUENCE_STORAGE_ITEM_H_
#define NLIVE_SEQUENCE_STORAGE_ITEM_H_

#include "model/storage/storage_item.h"
#include "model/sequence/sequence.h"

namespace nlive {

class SequenceStorageItem : public StorageItem {

private:
  QSharedPointer<Sequence> sequence_;

public:
  static const std::string TYPE;

  SequenceStorageItem(
    QSharedPointer<StorageItem> parent,
    QSharedPointer<Sequence> sequence);

  QSharedPointer<Clip> cliperize(QSharedPointer<Sequence> sequence) override;

  inline QSharedPointer<Sequence> sequence() { return sequence_; }

  inline bool is_directory() const { return false; }

};

}

#endif