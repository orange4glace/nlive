#ifndef NLIVE_STORAGE_DIRECTORY_H_
#define NLIVE_STORAGE_DIRECTORY_H_

#include <QSharedPointer>
#include <vector>
#include <boost/serialization/base_object.hpp>
#include "model/storage/storage_item.h"

namespace nlive {

class StorageDirectory : public StorageItem {

private:
  std::vector<QSharedPointer<StorageItem>> items_;

public:
  // friend class boost::serialization::access;
  // template <class Archive>
  // void serialize(Archive& ar, const unsigned int version) {
  //   ar << boost::serialization::base_object<StorageItem>(*this);
  // }

  StorageDirectory(
    QSharedPointer<Project> project,
    QString name,
    QSharedPointer<StorageItem> parent = nullptr,
    std::string uuid = std::string());
  StorageDirectory(
    QString name,
    QSharedPointer<StorageItem> parent = nullptr,
    std::string uuid = std::string());
  
  void addItem(QSharedPointer<StorageItem> item);
  void removeItem(QSharedPointer<StorageItem> item);
  int getItemIndex(QSharedPointer<StorageItem> item) const;
  bool hasItem(QSharedPointer<StorageItem> item) const;

  QSharedPointer<Clip> cliperize(QSharedPointer<Sequence> sequence) override;

  const std::vector<QSharedPointer<StorageItem>>& items();
  bool is_directory() const override;

  sig2_t<void (QSharedPointer<StorageItem> item, int index)> onDidAddItem;
  sig2_t<void (QSharedPointer<StorageItem> item, int index)> onWillRemoveItem;

};

}

#endif