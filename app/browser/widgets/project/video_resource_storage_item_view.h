#ifndef _NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_

#include <QOpenGLWidget>

#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"
#include "model/storage/video_resource_storage_item.h"
#include "renderer/video_renderer/renderer.h"

#include "renderer/video_renderer/renderer_view.h"

namespace nlive {

namespace project_widget {

class VideoResourceStorageItemView : public StorageItemView {

private:
  video_renderer::RendererView* renderer_view_;

  void doResize();

protected:
  void onScrubStart() override;
  void onScrub(double x) override;
  void onScrubEnd() override;
  
  void contentRectUpdated() override;
  void paintEvent(QPaintEvent* event) override;

public:
  VideoResourceStorageItemView(QWidget* parent, QSharedPointer<VideoResourceStorageItem> item, QSharedPointer<IThemeService> theme_service);

};

class VideoResourceStorageItemViewFactory : public StorageItemViewFactory {

public:
  VideoResourceStorageItemViewFactory();
  StorageItemView* create(QWidget* widget, QSharedPointer<StorageItem> item, QSharedPointer<IThemeService> theme_service) override;

};

}

}

#endif