#ifndef NLIVE_STORAGE_ITEM_VIEW_FACTORY_H_
#define NLIVE_STORAGE_ITEM_VIEW_FACTORY_H_

#include <string>
#include <map>

#include "platform/theme/themeservice.h"
#include "platform/service/service_locator.h"

class QWidget;

namespace nlive {

class StorageItem;

namespace project_widget {

class StorageItemView;

class StorageItemViewFactory {

protected:
  sptr<ServiceLocator> service_locator_;

public:
  StorageItemViewFactory(sptr<ServiceLocator> service_locator);
  virtual StorageItemView* create(QWidget* widget, sptr<StorageItem> item) = 0;

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