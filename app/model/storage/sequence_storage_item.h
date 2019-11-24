#ifndef NLIVE_SEQUENCE_STORAGE_ITEM_H_
#define NLIVE_SEQUENCE_STORAGE_ITEM_H_

#include "model/storage/storage_item.h"
#include "model/sequence/sequence.h"

namespace nlive {

class SequenceStorageItem : public StorageItem {

private:
  SequenceStorageItem() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<StorageItem>(*this);
  }
  
  sptr<Sequence> sequence_;

public:
  static const std::string TYPE;

  SequenceStorageItem(
    sptr<StorageItem> parent,
    QString name,
    sptr<Sequence> sequence);

  sptr<Clip> cliperize(sptr<Sequence> sequence) override;

  inline sptr<Sequence> sequence() { return sequence_; }

  inline bool is_directory() const { return false; }

};

}

#endif