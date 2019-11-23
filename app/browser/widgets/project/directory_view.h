#ifndef NLIVE_PROJECT_WIDGET_DIRECTORY_VIEW_H_
#define NLIVE_PROJECT_WIDGET_DIRECTORY_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include <vector>
#include <map>

#include "base/layout/div.h"
#include "base/layout/grid_layout.h"

namespace nlive {

class IThemeService;
class IImportService;
class StorageItem;
class StorageDirectory;

namespace project_widget {
  
class StorageItemView;

class DirectoryView : public Div {

private:
  sptr<IThemeService> theme_service_;
  sptr<IImportService> import_service_;
  sptr<StorageDirectory> storage_directory_;

  std::vector<std::pair<sptr<StorageItem>, StorageItemView*>> view_items_;
  
  GridLayout* grid_layout_;

  void addStorageItemView(sptr<StorageItem> storage_item, int index);
  void removeStorageItemView(sptr<StorageItem> storage_item);
  StorageItemView* getStorageItemView(sptr<StorageItem> storage_item);

protected:
  void contentRectUpdated() override;
  void paintEvent(QPaintEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;

public:
  DirectoryView(
    QWidget* parent,
    sptr<StorageDirectory> storage_directory,
    sptr<IThemeService> theme_service,
    sptr<IImportService> import_service);

  sptr<StorageDirectory> storage_directory();
  
};

}

}

#endif