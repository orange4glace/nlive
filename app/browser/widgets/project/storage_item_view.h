#ifndef _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_

#include <QWidget>

#include "platform/theme/themeservice.h"

namespace nlive {

class StorageItem;

namespace project_widget {

class StorageItemView : public QWidget {

private:
  QColor col_;

protected:
  QSharedPointer<IThemeService> theme_service_;

  StorageItem* storage_item_;

  void mousePressEvent(QMouseEvent* event) override;

public:
  StorageItemView(QWidget* parent, StorageItem* storage_item, QSharedPointer<IThemeService> theme_service);

  StorageItem* storage_item();
  
};

}

}

#endif