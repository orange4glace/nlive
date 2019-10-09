#ifndef _NLIVE_STORAGE_ITEM_VIEW_FACTORY_H_
#define _NLIVE_STORAGE_ITEM_VIEW_FACTORY_H_

#include <string>
#include <map>

#include "platform/theme/themeservice.h"

class QWidget;

namespace nlive {

class StorageItem;

namespace project_widget {

class StorageItemView;

class StorageItemViewFactory {

public:
  StorageItemViewFactory();
  virtual StorageItemView* create(QWidget* widget, StorageItem* item, QSharedPointer<IThemeService> theme_service) = 0;

};

class StorageItemViewFactoryRegistry {

private:
  static StorageItemViewFactory* default_factory_;
  static std::map<std::string, StorageItemViewFactory*> factories_;

public:
  static void registerDefaultFactory(StorageItemViewFactory* factory);
  static void registerFactory(std::string type, StorageItemViewFactory* factory);
  static StorageItemViewFactory* getFactory(std::string type);

};

}

}

#endif