#include "model/storage/storage_item.h"

#include <QDebug>
#include "base/common/uuid.h"

namespace nlive {

StorageItem::StorageItem(
  std::string type,
  QString name,
  QSharedPointer<StorageItem> parent,
  std::string uuid) :
  type_(type), name_(name), parent_(nullptr), uuid_(uuid) {
}

void StorageItem::setParent(StorageItem* item) {
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

StorageItem* StorageItem::parent() {
  return parent_;
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