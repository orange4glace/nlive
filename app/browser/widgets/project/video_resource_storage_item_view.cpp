#include "browser/widgets/project/video_resource_storage_item_view.h"

#include <QPainter>

#include "platform/logger/logger.h"

namespace nlive {

namespace project_widget {

VideoResourceStorageItemView::VideoResourceStorageItemView(QWidget* parent, VideoResourceStorageItem* item, IThemeService* theme_service) :
  StorageItemView(parent, item, theme_service) {

}

void VideoResourceStorageItemView::paintEvent(QPaintEvent* event) {
  auto theme = theme_service_->getTheme();
  QPainter p(this);
  p.setPen(QPen(theme.surfaceBrightColor()));
  p.drawRect(rect().marginsRemoved(QMargins(0, 0, 1, 1)));
}

VideoResourceStorageItemViewFactory::VideoResourceStorageItemViewFactory() {}
StorageItemView* VideoResourceStorageItemViewFactory::create(QWidget* parent, StorageItem* item, IThemeService* theme_service) {
  if (item->type() != VideoResourceStorageItem::TYPE) {
    spdlog::get(LOGGER_DEFAULT)->warn(
      "[VideoResourceStorageItemViewFactory] StorageItem type not match! expected={}, got={}", VideoResourceStorageItem::TYPE, item->type());
    return nullptr;
  }
  VideoResourceStorageItem* vrsi = static_cast<VideoResourceStorageItem*>(item);
  return new VideoResourceStorageItemView(parent, vrsi, theme_service);
}

}

}