#include "model/storage/resource_storage_item.h"

#include "model/sequence/clip.h"

namespace nlive {

ResourceStorageItem::ResourceStorageItem(
  std::string type,
  sptr<StorageItem> parent,
  sptr<Resource> resource) :
  StorageItem(parent->project(), type, resource->name(), parent),
  resource_(resource) {

}

sptr<Resource> ResourceStorageItem::resource() {
  return resource_;
}

}