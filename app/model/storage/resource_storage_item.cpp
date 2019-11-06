#include "model/storage/resource_storage_item.h"

#include "model/sequence/clip.h"

namespace nlive {

ResourceStorageItem::ResourceStorageItem(
  std::string type,
  QSharedPointer<StorageItem> parent,
  QSharedPointer<Resource> resource) :
  StorageItem(type, resource->name(), parent),
  resource_(resource) {

}

QSharedPointer<Resource> ResourceStorageItem::resource() {
  return resource_;
}

}