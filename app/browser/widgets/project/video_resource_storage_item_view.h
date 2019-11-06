#ifndef _NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_VIDEO_RESOURCE_STORAGE_ITEM_VIEW_H_

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
  VideoResourceStorageItemThumbnailView(QWidget* parent, QSharedPointer<VideoResourceStorageItem> item);
  ~VideoResourceStorageItemThumbnailView();

};

class VideoResourceStorageItemContentView : public StorageItemContentView {

private:
  QSharedPointer<StorageItem> item_;
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
  VideoResourceStorageItemContentView(QWidget* parent, QSharedPointer<VideoResourceStorageItem> item);

};

class VideoResourceStorageItemView : public StorageItemView {

private:
  VideoResourceStorageItemContentView* content_view_;

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