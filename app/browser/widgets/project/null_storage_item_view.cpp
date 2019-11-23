#include "browser/widgets/project/null_storage_item_view.h"

namespace nlive {

namespace project_widget {

NullStorageItemView::NullStorageItemView(QWidget* parent, sptr<StorageItem> item, sptr<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {

}

NullStorageItemViewFactory::NullStorageItemViewFactory(sptr<ServiceLocator> service_locator) :
  StorageItemViewFactory(service_locator) {}
StorageItemView* NullStorageItemViewFactory::create(QWidget* parent, sptr<StorageItem> item) {
  return new NullStorageItemView(parent, item,
      service_locator_->getService<IThemeService>(IThemeService::ID));
}

}

}