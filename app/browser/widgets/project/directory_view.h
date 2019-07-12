#ifndef _NLIVE_PROJECT_WIDGET_DIRECTORY_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_DIRECTORY_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <vector>
#include <map>

#include "base/layout/grid_layout.h"

namespace nlive {

class IThemeService;
class IImportService;
class StorageItem;
class StorageDirectory;

namespace project_widget {
  
class StorageItemView;

class DirectoryView : public QWidget {

private:
  IThemeService* theme_service_;
  IImportService* import_service_;
  QSharedPointer<StorageDirectory> storage_directory_;

  std::vector<std::pair<StorageItem*, StorageItemView*>> view_items_;
  
  GridLayout grid_layout_;

  void addStorageItemView(StorageItem* storage_item, int index);
  void removeStorageItemView(StorageItem* storage_item);
  StorageItemView* getStorageItemView(StorageItem* storage_item);

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;

public:
  DirectoryView(QWidget* parent,
    QSharedPointer<StorageDirectory> storage_directory,
    IThemeService* theme_service,
    IImportService* import_service);

  QSharedPointer<StorageDirectory> storage_directory();
  
};

}

}

#endif