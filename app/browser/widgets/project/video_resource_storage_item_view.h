#ifndef _NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_

#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"
#include "model/storage/video_resource_storage_item.h"

namespace nlive {

namespace project_widget {

class VideoResourceStorageItemView : public StorageItemView {

protected:
  void paintEvent(QPaintEvent* event) override;

public:
  VideoResourceStorageItemView(QWidget* parent, VideoResourceStorageItem* item, IThemeService* theme_service);

};

class VideoResourceStorageItemViewFactory : public StorageItemViewFactory {

public:
  VideoResourceStorageItemViewFactory();
  StorageItemView* create(QWidget* widget, StorageItem* item, IThemeService* theme_service) override;

};

}

}

#endif