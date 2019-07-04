#include "browser/widgets/project/null_storage_item_view.h"

namespace nlive {

namespace project_widget {

NullStorageItemView::NullStorageItemView(QWidget* parent, StorageItem* item) :
  StorageItemView(parent, item) {

}

NullStorageItemViewFactory::NullStorageItemViewFactory() {}
StorageItemView* NullStorageItemViewFactory::create(QWidget* parent, StorageItem* item) {
  return new NullStorageItemView(parent, item);
}

}

}