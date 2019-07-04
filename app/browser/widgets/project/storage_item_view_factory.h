#ifndef _NLIVE_STORAGE_ITEM_VIEW_FACTORY_H_
#define _NLIVE_STORAGE_ITEM_VIEW_FACTORY_H_

#include <QString>
#include <map>

class QWidget;

namespace nlive {

class StorageItem;

namespace project_widget {

class StorageItemView;

class StorageItemViewFactory {

public:
  StorageItemViewFactory();
  virtual StorageItemView* create(QWidget* widget, StorageItem* item) = 0;

};

class StorageItemViewFactoryRegistry {

private:
  static StorageItemViewFactory* default_factory_;
  static std::map<QString, StorageItemViewFactory*> factories_;

public:
  static void registerDefaultFactory(StorageItemViewFactory* factory);
  static void registerFactory(QString type, StorageItemViewFactory* factory);
  static StorageItemViewFactory* getFactory(QString type);

};

}

}

#endif