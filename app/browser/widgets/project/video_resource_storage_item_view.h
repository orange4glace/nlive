#ifndef NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_
#define NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_

#include <QOpenGLWidget>
#include <QImage>
#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"
#include "model/storage/video_resource_storage_item.h"
#include "renderer/video_renderer/renderer.h"

#include "renderer/video_renderer/renderer_view.h"

namespace nlive {

namespace project_widget {

class VideoResourceStorageItemThumbnailView : public Div {

private:
  QImage* image_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  VideoResourceStorageItemThumbnailView(QWidget* parent, sptr<VideoResourceStorageItem> item);
  ~VideoResourceStorageItemThumbnailView();

};

class VideoResourceStorageItemContentView : public StorageItemContentView {

private:
  sptr<StorageItem> item_;
  VideoResourceStorageItemThumbnailView* thumbnail_view_;
  video_renderer::RendererView* renderer_view_;

  void doLayout();

protected:
  void paintEvent(QPaintEvent* e) override;
  void contentRectUpdated() override;
  void onScrubStart() override;
  void onScrub(double x) override;
  void onScrubEnd() override;

public:
  VideoResourceStorageItemContentView(QWidget* parent, sptr<VideoResourceStorageItem> item);

};

class VideoResourceStorageItemView : public StorageItemView {

private:
  VideoResourceStorageItemContentView* content_view_;

public:
  VideoResourceStorageItemView(QWidget* parent, sptr<VideoResourceStorageItem> item, sptr<IThemeService> theme_service);

};

class VideoResourceStorageItemViewFactory : public StorageItemViewFactory {

public:
  VideoResourceStorageItemViewFactory(sptr<ServiceLocator> service_locator);
  StorageItemView* create(QWidget* widget, sptr<StorageItem> item) override;

};

}

}

#endif