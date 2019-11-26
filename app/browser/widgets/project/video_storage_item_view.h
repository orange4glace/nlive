#ifndef NLIVE_PROJECT_WIDGET_VIDEO_STORAGE_ITEM_VIEW_H_
#define NLIVE_PROJECT_WIDGET_VIDEO_STORAGE_ITEM_VIEW_H_

#include <QOpenGLWidget>
#include <QImage>
#include "browser/widgets/project/storage_item_view.h"
#include "browser/widgets/project/storage_item_view_factory.h"
#include "model/storage/video_storage_item.h"
#include "renderer/video_renderer/renderer.h"

#include "renderer/video_renderer/renderer_view.h"

namespace nlive {

namespace project_widget {

class VideoStorageItemThumbnailView : public Div {

private:
  QImage* image_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  VideoStorageItemThumbnailView(QWidget* parent, sptr<VideoStorageItem> item);
  ~VideoStorageItemThumbnailView();

};

class VideoStorageItemContentView : public StorageItemContentView {

private:
  sptr<StorageItem> item_;
  VideoStorageItemThumbnailView* thumbnail_view_;
  video_renderer::RendererView* renderer_view_;

  void doLayout();

protected:
  void paintEvent(QPaintEvent* e) override;
  void contentRectUpdated() override;
  void onScrubStart() override;
  void onScrub(double x) override;
  void onScrubEnd() override;

public:
  VideoStorageItemContentView(QWidget* parent, sptr<VideoStorageItem> item);

};

class VideoStorageItemView : public StorageItemView {

private:
  VideoStorageItemContentView* content_view_;

public:
  VideoStorageItemView(QWidget* parent, sptr<VideoStorageItem> item, sptr<IThemeService> theme_service);

};

class VideoStorageItemViewFactory : public StorageItemViewFactory {

public:
  VideoStorageItemViewFactory(sptr<ServiceLocator> service_locator);
  StorageItemView* create(QWidget* widget, sptr<StorageItem> item) override;

};

}

}

#endif