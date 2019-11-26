#include "browser/widgets/project/video_storage_item_renderer.h"

#include <QPainter>
#include "model/resource/video_resource.h"

namespace nlive {

namespace project_widget {

VideoStorageItemThumbnailView::VideoStorageItemThumbnailView(
  QWidget* parent, sptr<VideoStorageItem> item) :
  Div(parent) {
  sptr<VideoStorageItem> vrsi = std::static_pointer_cast<VideoStorageItem>(item);
  sptr<VideoResource> vs = vrsi->video_resource();
  VideoDecoder dec(item->video_resource()->path());
  auto frame = dec.decode(0, true);
  uint8_t* rgb = new uint8_t[vs->width() * vs->height() * 3];
  frame->scale(rgb, AV_PIX_FMT_RGB24);
  image_ = new QImage(rgb, vs->width(), vs->height(),
    vs->width() * 3, QImage::Format_RGB888);
}

VideoStorageItemThumbnailView::~VideoStorageItemThumbnailView() {
  delete image_;
}

void VideoStorageItemThumbnailView::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  p.drawImage(rect(), *image_);
}

VideoStorageItemRenderer::VideoStorageItemRenderer(
    sptr<VideoStorageItem> item,
    QOpenGLContext* target_gl) : item_(item), target_gl_(target_gl) {
  
}

void VideoStorageItemRenderer::open() {
  auto resource = item_->video_resource();
  renderer_ = sptr<video_renderer::Renderer>(
    new video_renderer::Renderer(target_gl_, resource->width(), resource->height()));
}

void VideoStorageItemRenderer::close() {
  renderer_ = nullptr;
}

}

}