#include "browser/widgets/project/video_resource_storage_item_renderer.h"

namespace nlive {

namespace project_widget {

VideoResourceStorageItemRenderer::VideoResourceStorageItemRenderer(
    QSharedPointer<VideoResourceStorageItem> item,
    QOpenGLContext* target_gl) : item_(item), target_gl_(target_gl) {
  
}

void VideoResourceStorageItemRenderer::open() {
  auto resource = item_->video_resource();
  renderer_ = QSharedPointer<video_renderer::Renderer>(
    new video_renderer::Renderer(target_gl_, resource->width(), resource->height()));
}

void VideoResourceStorageItemRenderer::close() {
  renderer_ = nullptr;
}

}

}