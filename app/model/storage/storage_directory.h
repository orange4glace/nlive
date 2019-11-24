#ifndef NLIVE_STORAGE_DIRECTORY_H_
#define NLIVE_STORAGE_DIRECTORY_H_

#include <QDebug>
#include <vector>
#include "base/common/memory.h"
#include "base/common/serialize.h"
#include "model/storage/storage_item.h"

namespace nlive {

class StorageDirectory : public StorageItem {

private:
  StorageDirectory() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<StorageItem>(*this);
    ar & items_;
  }

  std::vector<sptr<StorageItem>> items_;

public:
  static const std::string TYPE;
  
  StorageDirectory(
    sptr<Project> project,
    QString name,
    sptr<StorageItem> parent = nullptr);
  StorageDirectory(
    QString name,
    sptr<StorageItem> parent = nullptr);
  
  void addItem(sptr<StorageItem> item);
  void removeItem(sptr<StorageItem> item);
  int getItemIndex(sptr<StorageItem> item) const;
  bool hasItem(sptr<StorageItem> item) const;
  sptr<StorageItem> getChildItem(std::string uuid);
  sptr<StorageItem> getItem(std::string path);

  sptr<Clip> cliperize(sptr<Sequence> sequence) override;

  const std::vector<sptr<StorageItem>>& items();
  bool is_directory() const override;

  sig2_t<void (sptr<StorageItem> item, int index)> onDidAddItem;
  sig2_t<void (sptr<StorageItem> item, int index)> onWillRemoveItem;

};

}

#endif