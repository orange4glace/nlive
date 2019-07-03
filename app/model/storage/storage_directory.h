#ifndef _NLIVE_STORAGE_DIRECTORY_H_
#define _NLIVE_STORAGE_DIRECTORY_H_

#include <vector>
#include "model/storage/storage_item.h"

namespace nlive {

class StorageDirectory : public StorageItem {
  Q_OBJECT

private:
  std::vector<StorageItem*> items_;

public:
  StorageDirectory(QString name, StorageItem* parent = nullptr, QString uuid = QString());
  
  void addItem(StorageItem* item);
  void removeItem(StorageItem* item);
  int getItemIndex(StorageItem* item) const;
  bool hasItem(StorageItem* item) const;

  bool is_directory() const override;

signals:
  void onDidAddItem(StorageItem* item, int index);
  void onWillRemoveItem(StorageItem* item, int index);

};

}

#endif