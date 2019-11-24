#include "model/storage/storage_item.h"
#include "model/project/project.h"

#include <QDebug>
#include "base/common/uuid.h"

namespace nlive {

StorageItem::StorageItem(
  sptr<Project> project,
  std::string type,
  QString name,
  sptr<StorageItem> parent) :
  project_(project), type_(type), name_(name), parent_(parent),
  uuid_(UUID::instance()->generateUUID()) {
}

void StorageItem::setParent(sptr<StorageItem> item) {
  parent_ = item;
}

QString StorageItem::getAbsoluteNamePath() const {
  if (parent_ == nullptr) return name_;
  return parent_->getAbsoluteNamePath() + "/" + name_;
}

std::string StorageItem::getAbsolutePath() const {
  if (parent_ == nullptr) return uuid_;
  return parent_->getAbsolutePath() + "/" + uuid_;
}

std::string const& StorageItem::uuid() const {
  return uuid_;
}

QString const& StorageItem::name() const {
  return name_;
}

std::string const& StorageItem::type() const {
  return type_;
}

}