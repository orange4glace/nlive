#include "browser/widgets/project/storage_item_view.h"

#include "model/storage/storage_item.h"

namespace nlive {

namespace project_widget {

StorageItemView::StorageItemView(QWidget* parent, StorageItem* item) :
  QWidget(parent), storage_item_(item) {

}

StorageItem* StorageItemView::storage_item() {
  return storage_item_;
}

}

}