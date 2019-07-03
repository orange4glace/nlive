#ifndef _NLIVE_PROJECT_WIDGET_DIRECTORY_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_DIRECTORY_VIEW_H_

#include <QWidget>
#include <vector>
#include <map>

#include "base/layout/grid_layout.h"

namespace nlive {

class IThemeService;
class StorageItem;
class StorageDirectory;

namespace project_widget {
  
class StorageItemView;

class DirectoryView : public QWidget {

private:
  IThemeService* theme_service_;
  StorageDirectory* storage_directory_;

  std::vector<std::pair<StorageItem*, StorageItemView*>> view_items_;
  
  GridLayout grid_layout_;

  void addStorageItemView(StorageItem* storage_item, int index);
  void removeStorageItemView(StorageItem* storage_item);
  StorageItemView* getStorageItemView(StorageItem* storage_item);

protected:
  void paintEvent(QPaintEvent* event) override;

public:
  DirectoryView(QWidget* parent, StorageDirectory* storage_directory, IThemeService* theme_service);

  StorageDirectory* storage_directory();
  
};

}

}

#endif