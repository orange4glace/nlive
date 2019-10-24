#ifndef NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_RENDERER_H_
#define NLIVE_VIDEO_RESOURCE_STORAGE_ITEM_RENDERER_H_

#include <QOpenGLContext>
#include <QSharedPointer>
#include <mutex>

#include "renderer/video_renderer/renderer.h"
#include "model/storage/video_resource_storage_item.h"

namespace nlive {

namespace project_widget {

class VideoResourceStorageItemRenderer {

private:
  QSharedPointer<VideoResourceStorageItem> item_;
  QOpenGLContext* target_gl_;

  QSharedPointer<video_renderer::Renderer> renderer_;

  std::mutex mutex_;

public:
  VideoResourceStorageItemRenderer(
    QSharedPointer<VideoResourceStorageItem> item,
    QOpenGLContext* target_gl);

  void open();
  void close();

  void paint();

};

}

}

#endif