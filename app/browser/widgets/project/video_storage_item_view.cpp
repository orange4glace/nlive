#include "browser/widgets/project/video_storage_item_view.h"

#include <QDebug>
#include <QPainter>

#include "base/common/datetime.h"
#include "platform/logger/logger.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/video_renderer/video_clip_render_command.h"
#include "renderer/video_renderer/command/transform_blit_render_command.h"

namespace nlive {

namespace project_widget {

VideoStorageItemThumbnailView::VideoStorageItemThumbnailView(
  QWidget* parent, sptr<VideoStorageItem> item) :
  Div(parent) {
  sptr<VideoStorageItem> vrsi = std::static_pointer_cast<VideoStorageItem>(item);
  sptr<VideoResource> vs = vrsi->video_resource();
  VideoDecoder dec(item->video_resource()->path());
  auto frame = dec.decode(0, true);
  uint8_t* rgb = new uint8_t[vs->width() * vs->height() * 4];
  frame->scale(rgb, AV_PIX_FMT_RGB32);
  image_ = new QImage(rgb, vs->width(), vs->height(),
    vs->width() * 4, QImage::Format_RGBA8888);
}

VideoStorageItemThumbnailView::~VideoStorageItemThumbnailView() {
  delete image_;
}

void VideoStorageItemThumbnailView::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  p.drawImage(rect(), *image_);
}






VideoStorageItemContentView::VideoStorageItemContentView(
  QWidget* parent, sptr<VideoStorageItem> item) :
  StorageItemContentView(parent), item_(item) {
  thumbnail_view_ = new VideoStorageItemThumbnailView(this, item);
  renderer_view_ = nullptr;
}

void VideoStorageItemContentView::onScrubStart() {
  auto resource = std::static_pointer_cast<VideoStorageItem>(item_)->video_resource();
  renderer_view_ = new video_renderer::RendererView(this, resource->width(), resource->height());
  renderer_view_->setAttribute(Qt::WA_TransparentForMouseEvents);
  renderer_view_->show();
  doLayout();
}

void VideoStorageItemContentView::onScrub(double x) {
  auto command_buffer = sptr<video_renderer::CommandBuffer>(
    new video_renderer::CommandBuffer());
  auto resource = std::static_pointer_cast<VideoStorageItem>(item_)->video_resource();
  int64_t time = resource->duration() * x;
  
  auto pre = sptr<video_renderer::VideoClipPreRenderCommand>(
      new video_renderer::VideoClipPreRenderCommand(0, resource, time, true));
  auto blit = sptr<video_renderer::TransformBlitRenderCommand>(
      new video_renderer::TransformBlitRenderCommand(0, 0));
  auto post = sptr<video_renderer::VideoClipPostRenderCommand>(
      new video_renderer::VideoClipPostRenderCommand(pre->sharing, resource));
  command_buffer->addCommand(pre);
  command_buffer->addCommand(blit);
  command_buffer->addCommand(post);

  renderer_view_->render(command_buffer);
}

void VideoStorageItemContentView::onScrubEnd() {
  renderer_view_->deleteLater();
  renderer_view_ = nullptr;
}

void VideoStorageItemContentView::paintEvent(QPaintEvent* e) {

}

void VideoStorageItemContentView::contentRectUpdated() {
  StorageItemContentView::contentRectUpdated();
  doLayout();
}

void VideoStorageItemContentView::doLayout() {
  auto resource = std::static_pointer_cast<VideoStorageItem>(item_)->video_resource();
  double r = (double) resource->width() / resource->height();
  int w = height() * r;
  int h = height();
  double xx = (double)width() / w;
  double yy = (double)height() / h;
  double rr = min(1.0, min(xx, yy));
  w = w * rr;
  h = h * rr;
  setChildGeometry(thumbnail_view_,
    (width() - w) / 2 + 1,
    (height() - h) / 2 + 1,
    w - 2, h - 2);
  if (renderer_view_ != nullptr) {
    setChildGeometry(renderer_view_,
      (width() - w) / 2 + 1,
      (height() - h) / 2 + 1,
      w - 2, h - 2);
  }
}

// void VideoStorageItemContentView::paintEvent(QPaintEvent* event) {
//   auto theme = theme_service_->getTheme();
//   QPainter p(this);
//   p.setPen(QPen(theme.surfaceBrightColor()));
//   p.drawRect(rect().marginsRemoved(QMargins(0, 0, 1, 1)));
// }




VideoStorageItemView::VideoStorageItemView(QWidget* parent, sptr<VideoStorageItem> item, sptr<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {
  sptr<VideoStorageItem> vrsi = std::static_pointer_cast<VideoStorageItem>(item);
  QString dur = QString::fromStdString(datetime::secondsToHHmmss(vrsi->video_resource()->duration_in_seconds()));
  title_view()->left_label_box()->setText(item->name());
  title_view()->right_label_box()->setText(dur);
  content_view_ = new VideoStorageItemContentView(nullptr, vrsi);
  content_view()->setContentView(content_view_);
}

VideoStorageItemViewFactory::VideoStorageItemViewFactory(sptr<ServiceLocator> service_locator) :
  StorageItemViewFactory(service_locator) {}
StorageItemView* VideoStorageItemViewFactory::create(QWidget* parent, sptr<StorageItem> item) {
  Q_ASSERT(item->type() == VideoStorageItem::TYPE);
  sptr<VideoStorageItem> vrsi = std::static_pointer_cast<VideoStorageItem>(item);
  return new VideoStorageItemView(parent, vrsi, service_locator_->getService<IThemeService>(IThemeService::ID));
}

}

}