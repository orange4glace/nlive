#include "model/storage/storage_directory.h"
#include "platform/logger/logger.h"

namespace nlive {
  
const std::string StorageDirectory::TYPE = "nlive.model.storage_item.StorageDirectory";

StorageDirectory::StorageDirectory(
  sptr<Project> project,
  QString name,
  sptr<StorageItem> parent) :
  StorageItem(project, TYPE, name, parent) {
}

StorageDirectory::StorageDirectory(
  QString name,
  sptr<StorageItem> parent) :
  StorageItem(parent->project(), TYPE, name, parent) {
}

void StorageDirectory::addItem(sptr<StorageItem> item) {
  // TODO : How to prevent multiple parents has a same StorageItem as child?
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

sptr<StorageItem> StorageDirectory::getChildItem(std::string uuid) {
  // TODO : logN query
  for (auto it : items_) {
    if (it->uuid() == uuid) return it;
  }
  spdlog::get(LOGGER_DEFAULT)->warn("[StorageDirectory] Can't get item at \"{}\"! Child not found. Path = \"{}\"",
    uuid, getAbsolutePath());
  return nullptr;
}

sptr<StorageItem> StorageDirectory::getItem(std::string path) {
  size_t del_pos = path.find('/');
  if (del_pos == std::string::npos) {
    return getChildItem(path);
  }
  else {
    std::string child_uuid = path.substr(0, del_pos);
    std::string remaining_path = path.substr(del_pos + 1, std::string::npos);
    auto child = getChildItem(child_uuid);
    if (child == nullptr) return nullptr;
    if (child->type() != StorageDirectory::TYPE) {
      spdlog::get(LOGGER_DEFAULT)->error("[StorageDirectory] Can't get item at \"{}\"! Type mismatch, Type = \"{}\". Path = \"{}\"",
        path, child->type(), getAbsolutePath());
      return nullptr;
    }
    return std::static_pointer_cast<StorageDirectory>(child)->getItem(remaining_path);
  }
  return nullptr;
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