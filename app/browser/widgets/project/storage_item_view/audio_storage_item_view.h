#ifndef NLIVE_PROJECT_WIDGET_AUDIO_STORAGE_ITEM_VIEW_H_
#define NLIVE_PROJECT_WIDGET_AUDIO_STORAGE_ITEM_VIEW_H_

#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"
#include "model/storage/audio_storage_item.h"

namespace nlive {

namespace project_widget {

class AudioStorageItemView : public StorageItemView {

public:
  AudioStorageItemView(QWidget* parent, sptr<AudioStorageItem> item, sptr<IThemeService> theme_service);

};

class AudioStorageItemViewFactory : public StorageItemViewFactory {

public:
  AudioStorageItemViewFactory(sptr<ServiceLocator> service_locator);
  StorageItemView* create(QWidget* widget, sptr<StorageItem> item) override;

};

}

}

#endif