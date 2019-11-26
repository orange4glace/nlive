#ifndef NLIVE_VIDEO_STORAGE_ITEM_RENDERER_H_
#define NLIVE_VIDEO_STORAGE_ITEM_RENDERER_H_

#include <QOpenGLContext>
#include "base/common/memory.h"
#include <QImage>
#include <mutex>

#include "base/layout/div.h"
#include "renderer/video_renderer/renderer.h"
#include "model/storage/video_storage_item.h"
#include "decoder/video_decoder.h"

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

class VideoStorageItemRenderer {

private:
  sptr<VideoStorageItem> item_;
  QOpenGLContext* target_gl_;

  sptr<video_renderer::Renderer> renderer_;

  std::mutex mutex_;

public:
  VideoStorageItemRenderer(
    sptr<VideoStorageItem> item,
    QOpenGLContext* target_gl);

  void open();
  void close();

  void paint();

};

}

}

#endif