#ifndef NLIVE_PROJECT_WIDGET_NULL_STORAGE_ITEM_VIEW_H_
#define NLIVE_PROJECT_WIDGET_NULL_STORAGE_ITEM_VIEW_H_

#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"

namespace nlive {

namespace project_widget {

class NullStorageItemView : public StorageItemView {

public:
  NullStorageItemView(QWidget* parent, sptr<StorageItem> item, sptr<IThemeService> theme_service);

};

class NullStorageItemViewFactory : public StorageItemViewFactory {

public:
  NullStorageItemViewFactory(sptr<ServiceLocator> service_locator);
  StorageItemView* create(QWidget* widget, sptr<StorageItem> item) override;

};

}

}

#endif