#ifndef _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_

#include <QWidget>

namespace nlive {

class IThemeService;
class StorageItem;

namespace project_widget {

class StorageItemView : public QWidget {

private:
  QColor col_;

protected:
  StorageItem* storage_item_;

  void mousePressEvent(QMouseEvent* event) override;

  void paintEvent(QPaintEvent* event) override;

public:
  StorageItemView(QWidget* parent, StorageItem* storage_item);

  StorageItem* storage_item();
  
};

}

}

#endif