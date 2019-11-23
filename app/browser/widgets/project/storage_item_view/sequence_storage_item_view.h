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
  QSharedPointer<IWidgetsService> widgets_service_;
  QSharedPointer<SequenceStorageItem> item_;

protected:
  void onMouseDoubleClick() override;

public:
  SequenceStorageItemView(QWidget* parent, QSharedPointer<SequenceStorageItem> item,
      QSharedPointer<IThemeService> theme_service, QSharedPointer<IWidgetsService> widgets_service);

};

class SequenceStorageItemViewFactory : public StorageItemViewFactory {

public:
  SequenceStorageItemViewFactory(QSharedPointer<ServiceLocator> service_locator);
  StorageItemView* create(QWidget* widget, QSharedPointer<StorageItem> item) override;

};

}

}

#endif