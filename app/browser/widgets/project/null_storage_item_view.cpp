#include "browser/widgets/project/null_storage_item_view.h"

namespace nlive {

namespace project_widget {

NullStorageItemView::NullStorageItemView(QWidget* parent, QSharedPointer<StorageItem> item, QSharedPointer<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {

}

NullStorageItemViewFactory::NullStorageItemViewFactory(QSharedPointer<ServiceLocator> service_locator) :
  StorageItemViewFactory(service_locator) {}
StorageItemView* NullStorageItemViewFactory::create(QWidget* parent, QSharedPointer<StorageItem> item) {
  return new NullStorageItemView(parent, item,
      service_locator_->getService<IThemeService>(IThemeService::ID));
}

}

}