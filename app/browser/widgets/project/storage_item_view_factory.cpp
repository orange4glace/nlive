#include "browser/widgets/project/storage_item_view_factory.h"

namespace nlive {

namespace project_widget {

StorageItemViewFactory* StorageItemViewFactoryRegistry::default_factory_ = nullptr;
std::map<QString, StorageItemViewFactory*> StorageItemViewFactoryRegistry::factories_;

StorageItemViewFactory::StorageItemViewFactory() {}

void StorageItemViewFactoryRegistry::registerDefaultFactory(StorageItemViewFactory* factory) {
  default_factory_ = factory;
}

void StorageItemViewFactoryRegistry::registerFactory(QString type, StorageItemViewFactory* factory) {
  factories_.emplace(type, factory);
}

StorageItemViewFactory* StorageItemViewFactoryRegistry::getFactory(QString type) {
  if (factories_.count(type) == 0) return default_factory_;
  return factories_[type];
}

}

}