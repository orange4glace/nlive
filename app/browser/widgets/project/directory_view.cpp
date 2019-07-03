#include "browser/widgets/project/directory_view.h"

#include "platform/theme/themeservice.h"
#include "model/storage/storage_directory.h"
#include "model/storage/storage_item.h"
#include "browser/widgets/project/storage_item_view.h"

namespace nlive {

namespace project_widget {

DirectoryView::DirectoryView(QWidget* parent, StorageDirectory* storage_directory, IThemeService* theme_service) :
  theme_service_(theme_service), QWidget(parent), storage_directory_(storage_directory), grid_layout_(this) {

}

void DirectoryView::addStorageItemView(StorageItem* storage_item, int index) {
  auto view = new StorageItemView(&grid_layout_, storage_item);
  auto pair = make_pair(storage_item, view);
  view_items_.insert(view_items_.begin() + index, pair);
}

void DirectoryView::removeStorageItemView(StorageItem* storage_item) {
  int i = 0;
  for (auto it : view_items_) {
    if (it.first == storage_item) break;
    i++;
  }
  if (i == -1) return;
  view_items_.erase(view_items_.begin() + i);
}

StorageItemView* DirectoryView::getStorageItemView(StorageItem* storage_item) {
  for (auto it : view_items_)
    if (it.first == storage_item) return it.second;
  return nullptr;
}

StorageDirectory* DirectoryView::storage_directory() {
  return storage_directory_;
}

}

}