#include "model/storage/storage_directory.h"

namespace nlive {

StorageDirectory::StorageDirectory(
  sptr<Project> project,
  QString name,
  sptr<StorageItem> parent,
  std::string uuid) :
  StorageItem(project, "DIRECTORY", name, parent, uuid) {
}

StorageDirectory::StorageDirectory(
  QString name,
  sptr<StorageItem> parent,
  std::string uuid) :
  StorageItem(parent->project(), "DIRECTORY", name, parent, uuid) {
}

void StorageDirectory::addItem(sptr<StorageItem> item) {
  if (hasItem(item)) return;
  items_.push_back(item);
  emit onDidAddItem(item, items_.size() - 1);
}

void StorageDirectory::removeItem(sptr<StorageItem> item) {
  int index = getItemIndex(item);
  if (index == -1) return;
  emit onWillRemoveItem(item, index);
  items_.erase(items_.begin() + index);
}

int StorageDirectory::getItemIndex(sptr<StorageItem> item) const {
  // TODO : logN query
  int i = 0;
  for (auto it : items_) {
    if (it == item) return i;
    i++;
  }
  return -1;
}

bool StorageDirectory::hasItem(sptr<StorageItem> item) const {
  return getItemIndex(item) != -1;
}

sptr<Clip> StorageDirectory::cliperize(sptr<Sequence> sequence) {
  // TODO
  return nullptr;
}

const std::vector<sptr<StorageItem>>& StorageDirectory::items() {
  return items_;
}

bool StorageDirectory::is_directory() const {
  return true;
}

}