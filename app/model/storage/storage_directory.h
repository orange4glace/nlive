#ifndef NLIVE_STORAGE_DIRECTORY_H_
#define NLIVE_STORAGE_DIRECTORY_H_

#include <QSharedPointer>
#include <vector>
#include "model/storage/storage_item.h"

namespace nlive {

class StorageDirectory : public StorageItem {
  Q_OBJECT

private:
  std::vector<QSharedPointer<StorageItem>> items_;

public:
  StorageDirectory(
    QString name,
    QSharedPointer<StorageItem> parent = nullptr,
    std::string uuid = std::string());
  
  void addItem(QSharedPointer<StorageItem> item);
  void removeItem(QSharedPointer<StorageItem> item);
  int getItemIndex(QSharedPointer<StorageItem> item) const;
  bool hasItem(QSharedPointer<StorageItem> item) const;

  QSharedPointer<Clip> cliperize(Rational time_base) override;

  const std::vector<QSharedPointer<StorageItem>>& items();
  bool is_directory() const override;

signals:
  void onDidAddItem(QSharedPointer<StorageItem> item, int index);
  void onWillRemoveItem(QSharedPointer<StorageItem> item, int index);

};

}

#endif