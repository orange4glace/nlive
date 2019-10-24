#include "browser/widgets/project/video_resource_storage_item_view.h"

#include <QDebug>
#include <QPainter>

#include "platform/logger/logger.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/video_renderer/video_clip_render_command.h"
#include "renderer/video_renderer/command/transform_blit_render_command.h"

namespace nlive {

namespace project_widget {

VideoResourceStorageItemView::VideoResourceStorageItemView(QWidget* parent, QSharedPointer<VideoResourceStorageItem> item, QSharedPointer<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {
  renderer_view_ = nullptr;
}

void VideoResourceStorageItemView::resize() {
  if (renderer_view_ != nullptr) {
    auto resource = storage_item_.staticCast<VideoResourceStorageItem>()->video_resource();
    double r = (double) resource->width() / resource->height();
    int w = height() * r;
    int h = height();
    double xx = (double)width() / w;
    double yy = (double)height() / h;
    double rr = min(1.0, min(xx, yy));
    w = w * rr;
    h = h * rr;
    renderer_view_->setGeometry(
      (width() - w) / 2 + 1,
      (height() - h) / 2 + 1,
      w - 2, h - 2);
  }
}

void VideoResourceStorageItemView::onScrubStart() {
  auto resource = storage_item_.staticCast<VideoResourceStorageItem>()->video_resource();
  renderer_view_ = new video_renderer::RendererView(this, resource->width(), resource->height());
  renderer_view_->setAttribute(Qt::WA_TransparentForMouseEvents);
  renderer_view_->show();
  resize();
}

void VideoResourceStorageItemView::onScrub(double x) {
  auto command_buffer = QSharedPointer<video_renderer::CommandBuffer>(
    new video_renderer::CommandBuffer());
  auto resource = storage_item_.staticCast<VideoResourceStorageItem>()->video_resource();
  int64_t time = resource->duration() * x;
  
  auto pre = QSharedPointer<video_renderer::VideoClipPreRenderCommand>(
      new video_renderer::VideoClipPreRenderCommand(0, resource, time));
  auto blit = QSharedPointer<video_renderer::TransformBlitRenderCommand>(
      new video_renderer::TransformBlitRenderCommand(0, 0));
  auto post = QSharedPointer<video_renderer::VideoClipPostRenderCommand>(
      new video_renderer::VideoClipPostRenderCommand(pre->sharing, resource));
  command_buffer->addCommand(pre);
  command_buffer->addCommand(blit);
  command_buffer->addCommand(post);

  renderer_view_->render(command_buffer);
}

void VideoResourceStorageItemView::onScrubEnd() {
  renderer_view_->deleteLater();
  renderer_view_ = nullptr;
}

void VideoResourceStorageItemView::resizeEvent(QResizeEvent* event) {
  StorageItemView::resizeEvent(event);
  if (renderer_view_ != nullptr)
    resize();
}

void VideoResourceStorageItemView::paintEvent(QPaintEvent* event) {
  auto theme = theme_service_->getTheme();
  QPainter p(this);
  p.setPen(QPen(theme.surfaceBrightColor()));
  p.drawRect(rect().marginsRemoved(QMargins(0, 0, 1, 1)));
}

VideoResourceStorageItemViewFactory::VideoResourceStorageItemViewFactory() {}
StorageItemView* VideoResourceStorageItemViewFactory::create(QWidget* parent, QSharedPointer<StorageItem> item, QSharedPointer<IThemeService> theme_service) {
  if (item->type() != VideoResourceStorageItem::TYPE) {
    spdlog::get(LOGGER_DEFAULT)->warn(
      "[VideoResourceStorageItemViewFactory] StorageItem type not match! expected = {}, got = {}", VideoResourceStorageItem::TYPE, item->type());
    return nullptr;
  }
  QSharedPointer<VideoResourceStorageItem> vrsi = item.staticCast<VideoResourceStorageItem>();
  return new VideoResourceStorageItemView(parent, vrsi, theme_service);
}

}

}