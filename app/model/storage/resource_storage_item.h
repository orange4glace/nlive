#ifndef NLIVE_RESOURCE_STORAGE_ITEM_H_
#define NLIVE_RESOURCE_STORAGE_ITEM_H_

#include "base/common/memory.h"

#include "model/resource/resource.h"
#include "model/storage/storage_item.h"

namespace nlive {

class ResourceStorageItem : public StorageItem {

private:
  ResourceStorageItem() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<StorageItem>(*this);
  }

  sptr<Resource> resource_;

public:
  ResourceStorageItem(
    std::string type,
    sptr<StorageItem> parent,
    sptr<Resource> resource);

  sptr<Resource> resource();

  inline bool is_directory() const { return false; }

};

}

#endif