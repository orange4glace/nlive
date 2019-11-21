#include "browser/widgets/project/video_resource_storage_item_view.h"

#include <QDebug>
#include <QPainter>

#include "base/common/datetime.h"
#include "platform/logger/logger.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/video_renderer/video_clip_render_command.h"
#include "renderer/video_renderer/command/transform_blit_render_command.h"

namespace nlive {

namespace project_widget {

VideoResourceStorageItemThumbnailView::VideoResourceStorageItemThumbnailView(
  QWidget* parent, QSharedPointer<VideoResourceStorageItem> item) :
  Div(parent) {
  QSharedPointer<VideoResourceStorageItem> vrsi = item.staticCast<VideoResourceStorageItem>();
  QSharedPointer<VideoResource> vs = vrsi->video_resource();
  VideoDecoder dec(item->video_resource()->path());
  auto frame = dec.decode(0, true);
  uint8_t* rgb = new uint8_t[vs->width() * vs->height() * 4];
  frame->scale(rgb, AV_PIX_FMT_RGB32);
  image_ = new QImage(rgb, vs->width(), vs->height(),
    vs->width() * 4, QImage::Format_RGBA8888);
}

VideoResourceStorageItemThumbnailView::~VideoResourceStorageItemThumbnailView() {
  delete image_;
}

void VideoResourceStorageItemThumbnailView::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  p.drawImage(rect(), *image_);
}






VideoResourceStorageItemContentView::VideoResourceStorageItemContentView(
  QWidget* parent, QSharedPointer<VideoResourceStorageItem> item) :
  StorageItemContentView(parent), item_(item) {
  thumbnail_view_ = new VideoResourceStorageItemThumbnailView(this, item);
  renderer_view_ = nullptr;
}

void VideoResourceStorageItemContentView::onScrubStart() {
  auto resource = item_.staticCast<VideoResourceStorageItem>()->video_resource();
  renderer_view_ = new video_renderer::RendererView(this, resource->width(), resource->height());
  renderer_view_->setAttribute(Qt::WA_TransparentForMouseEvents);
  renderer_view_->show();
  doLayout();
}

void VideoResourceStorageItemContentView::onScrub(double x) {
  auto command_buffer = QSharedPointer<video_renderer::CommandBuffer>(
    new video_renderer::CommandBuffer());
  auto resource = item_.staticCast<VideoResourceStorageItem>()->video_resource();
  int64_t time = resource->duration() * x;
  
  auto pre = QSharedPointer<video_renderer::VideoClipPreRenderCommand>(
      new video_renderer::VideoClipPreRenderCommand(0, resource, time, true));
  auto blit = QSharedPointer<video_renderer::TransformBlitRenderCommand>(
      new video_renderer::TransformBlitRenderCommand(0, 0));
  auto post = QSharedPointer<video_renderer::VideoClipPostRenderCommand>(
      new video_renderer::VideoClipPostRenderCommand(pre->sharing, resource));
  command_buffer->addCommand(pre);
  command_buffer->addCommand(blit);
  command_buffer->addCommand(post);

  renderer_view_->render(command_buffer);
}

void VideoResourceStorageItemContentView::onScrubEnd() {
  renderer_view_->deleteLater();
  renderer_view_ = nullptr;
}

void VideoResourceStorageItemContentView::paintEvent(QPaintEvent* e) {

}

void VideoResourceStorageItemContentView::contentRectUpdated() {
  StorageItemContentView::contentRectUpdated();
  doLayout();
}

void VideoResourceStorageItemContentView::doLayout() {
  auto resource = item_.staticCast<VideoResourceStorageItem>()->video_resource();
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

// void VideoResourceStorageItemContentView::paintEvent(QPaintEvent* event) {
//   auto theme = theme_service_->getTheme();
//   QPainter p(this);
//   p.setPen(QPen(theme.surfaceBrightColor()));
//   p.drawRect(rect().marginsRemoved(QMargins(0, 0, 1, 1)));
// }




VideoResourceStorageItemView::VideoResourceStorageItemView(QWidget* parent, QSharedPointer<VideoResourceStorageItem> item, QSharedPointer<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {
  QSharedPointer<VideoResourceStorageItem> vrsi = item.staticCast<VideoResourceStorageItem>();
  QString dur = QString::fromStdString(datetime::secondsToHHmmss(vrsi->video_resource()->duration_in_seconds()));
  title_view()->left_label_box()->setText(item->name());
  title_view()->right_label_box()->setText(dur);
  content_view_ = new VideoResourceStorageItemContentView(nullptr, vrsi);
  content_view()->setContentView(content_view_);
}

VideoResourceStorageItemViewFactory::VideoResourceStorageItemViewFactory(QSharedPointer<ServiceLocator> service_locator) :
  StorageItemViewFactory(service_locator) {}
StorageItemView* VideoResourceStorageItemViewFactory::create(QWidget* parent, QSharedPointer<StorageItem> item) {
  Q_ASSERT(item->type() == VideoResourceStorageItem::TYPE);
  QSharedPointer<VideoResourceStorageItem> vrsi = item.staticCast<VideoResourceStorageItem>();
  return new VideoResourceStorageItemView(parent, vrsi, service_locator_->getService<IThemeService>(IThemeService::ID));
}

}

}