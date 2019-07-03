#ifndef _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_

#include <QWidget>

namespace nlive {

class IThemeService;
class StorageItem;

namespace project_widget {

class StorageItemView : public QWidget {

protected:
  StorageItem* storage_item_;

public:
  StorageItemView(QWidget* parent, StorageItem* storage_item);

  StorageItem* storage_item();
  
};

}

}

#endif