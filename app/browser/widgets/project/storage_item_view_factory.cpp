#include "browser/widgets/project/storage_item_view_factory.h"

namespace nlive {

namespace project_widget {

StorageItemViewFactory* StorageItemViewFactoryRegistry::default_factory_ = nullptr;
std::map<std::string, StorageItemViewFactory*> StorageItemViewFactoryRegistry::factories_;

StorageItemViewFactory::StorageItemViewFactory(QSharedPointer<ServiceLocator> service_locator) : 
  service_locator_(service_locator) {

}

void StorageItemViewFactoryRegistry::registerDefaultFactory(StorageItemViewFactory* factory) {
  default_factory_ = factory;
}

void StorageItemViewFactoryRegistry::registerFactory(std::string type, StorageItemViewFactory* factory) {
  factories_.emplace(type, factory);
}

StorageItemViewFactory* StorageItemViewFactoryRegistry::getFactory(std::string type) {
  if (factories_.count(type) == 0) return default_factory_;
  return factories_[type];
}

}

}