#include "browser/widgets/project/storage_item_view/audio_storage_item_view.h"

#include "base/common/datetime.h"

namespace nlive {

namespace project_widget {

AudioStorageItemView::AudioStorageItemView(QWidget* parent, QSharedPointer<AudioStorageItem> item, QSharedPointer<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {
  QSharedPointer<AudioStorageItem> asi = item.staticCast<AudioStorageItem>();
  QString dur = QString::fromStdString(datetime::secondsToHHmmss(asi->audio_resource()->duration_in_seconds()));
  title_view()->left_label_box()->setText(item->name());
  title_view()->right_label_box()->setText(dur);
}

AudioStorageItemViewFactory::AudioStorageItemViewFactory() {}
StorageItemView* AudioStorageItemViewFactory::create(QWidget* parent, QSharedPointer<StorageItem> item, QSharedPointer<IThemeService> theme_service) {
  Q_ASSERT(item->type() == AudioStorageItem::TYPE);
  QSharedPointer<AudioStorageItem> vrsi = item.staticCast<AudioStorageItem>();
  return new AudioStorageItemView(parent, vrsi, theme_service);
}

}

}