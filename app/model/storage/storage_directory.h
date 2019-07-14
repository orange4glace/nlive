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
  StorageDirectory(
    QString name,
    QSharedPointer<StorageItem> parent = nullptr,
    QString uuid = QString());
  
  void addItem(StorageItem* item);
  void removeItem(StorageItem* item);
  int getItemIndex(StorageItem* item) const;
  bool hasItem(StorageItem* item) const;

  QSharedPointer<Clip> cliperize(Rational time_base) override;

  const std::vector<StorageItem*>& items();
  bool is_directory() const override;

signals:
  void onDidAddItem(StorageItem* item, int index);
  void onWillRemoveItem(StorageItem* item, int index);

};

}

#endif