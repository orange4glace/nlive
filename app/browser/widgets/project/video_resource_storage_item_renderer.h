#ifndef NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_RENDERER_H_
#define NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_RENDERER_H_

#include <QOpenGLContext>
#include "base/common/memory.h"
#include <QImage>
#include <mutex>

#include "base/layout/div.h"
#include "renderer/video_renderer/renderer.h"
#include "model/storage/video_resource_storage_item.h"
#include "decoder/video_decoder.h"

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

class VideoResourceStorageItemRenderer {

private:
  sptr<VideoResourceStorageItem> item_;
  QOpenGLContext* target_gl_;

  sptr<video_renderer::Renderer> renderer_;

  std::mutex mutex_;

public:
  VideoResourceStorageItemRenderer(
    sptr<VideoResourceStorageItem> item,
    QOpenGLContext* target_gl);

  void open();
  void close();

  void paint();

};

}

}

#endif