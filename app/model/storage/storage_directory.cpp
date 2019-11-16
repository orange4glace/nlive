#include "model/storage/storage_directory.h"

namespace nlive {

StorageDirectory::StorageDirectory(
  QString name,
  QSharedPointer<StorageItem> parent,
  std::string uuid) :
  StorageItem("DIRECTORY", name, parent, uuid) {
}

void StorageDirectory::addItem(QSharedPointer<StorageItem> item) {
  if (hasItem(item)) return;
  items_.push_back(item);
  emit onDidAddItem(item, items_.size() - 1);
}

void StorageDirectory::removeItem(QSharedPointer<StorageItem> item) {
  int index = getItemIndex(item);
  if (index == -1) return;
  emit onWillRemoveItem(item, index);
  items_.erase(items_.begin() + index);
}

int StorageDirectory::getItemIndex(QSharedPointer<StorageItem> item) const {
  // TODO : logN query
  int i = 0;
  for (auto it : items_) {
    if (it == item) return i;
    i++;
  }
  return -1;
}

bool StorageDirectory::hasItem(QSharedPointer<StorageItem> item) const {
  return getItemIndex(item) != -1;
}

QSharedPointer<Clip> StorageDirectory::cliperize(QSharedPointer<Sequence> sequence) {
  // TODO
  return nullptr;
}

const std::vector<QSharedPointer<StorageItem>>& StorageDirectory::items() {
  return items_;
}

bool StorageDirectory::is_directory() const {
  return true;
}

}