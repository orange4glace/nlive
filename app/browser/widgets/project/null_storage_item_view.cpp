#include "browser/widgets/project/null_storage_item_view.h"

namespace nlive {

namespace project_widget {

NullStorageItemView::NullStorageItemView(QWidget* parent, StorageItem* item, IThemeService* theme_service) :
  StorageItemView(parent, item, theme_service) {

}

NullStorageItemViewFactory::NullStorageItemViewFactory() {}
StorageItemView* NullStorageItemViewFactory::create(QWidget* parent, StorageItem* item, IThemeService* theme_service) {
  return new NullStorageItemView(parent, item, theme_service);
}

}

}