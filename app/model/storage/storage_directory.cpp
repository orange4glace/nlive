#include "model/storage/storage_directory.h"

namespace nlive {

StorageDirectory::StorageDirectory(QString name, StorageItem* parent, QString uuid) :
  StorageItem(QString("DIRECTORY"), name, parent, uuid) {

}

void StorageDirectory::addItem(StorageItem* item) {
  if (hasItem(item)) return;
  items_.push_back(item);
  emit onDidAddItem(item, items_.size() - 1);
}

void StorageDirectory::removeItem(StorageItem* item) {
  int index = getItemIndex(item);
  if (index == -1) return;
  emit onWillRemoveItem(item, index);
  items_.erase(items_.begin() + index);
}

int StorageDirectory::getItemIndex(StorageItem* item) const {
  // TODO : logN query
  int i = 0;
  for (auto it : items_) {
    if (it == item) return i;
    i++;
  }
  return -1;
}

bool StorageDirectory::hasItem(StorageItem* item) const {
  return getItemIndex(item) != -1;
}

bool StorageDirectory::is_directory() const {
  return true;
}

}