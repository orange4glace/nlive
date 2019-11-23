#ifndef NLIVE_PROJECT_WIDGET_SEQUENCE_STORAGE_ITEM_VIEW_H_
#define NLIVE_PROJECT_WIDGET_SEQUENCE_STORAGE_ITEM_VIEW_H_

#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"
#include "model/storage/sequence_storage_item.h"
#include "browser/services/widgets/widgets_service.h"

namespace nlive {

namespace project_widget {

class SequenceStorageItemView : public StorageItemView {

private:
  sptr<IWidgetsService> widgets_service_;
  sptr<SequenceStorageItem> item_;

protected:
  void onMouseDoubleClick() override;

public:
  SequenceStorageItemView(QWidget* parent, sptr<SequenceStorageItem> item,
      sptr<IThemeService> theme_service, sptr<IWidgetsService> widgets_service);

};

class SequenceStorageItemViewFactory : public StorageItemViewFactory {

public:
  SequenceStorageItemViewFactory(sptr<ServiceLocator> service_locator);
  StorageItemView* create(QWidget* widget, sptr<StorageItem> item) override;

};

}

}

#endif