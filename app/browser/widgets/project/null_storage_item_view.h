#ifndef _NLIVE_PROJECT_WIDGET_NULL_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_NULL_STORAGE_ITEM_VIEW_H_

#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"

namespace nlive {

namespace project_widget {

class NullStorageItemView : public StorageItemView {

public:
  NullStorageItemView(QWidget* parent, StorageItem* item);

};

class NullStorageItemViewFactory : public StorageItemViewFactory {

public:
  NullStorageItemViewFactory();
  StorageItemView* create(QWidget* widget, StorageItem* item) override;

};

}

}

#endif