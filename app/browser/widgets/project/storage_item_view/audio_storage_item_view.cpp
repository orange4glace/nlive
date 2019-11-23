#include "browser/widgets/project/storage_item_view/audio_storage_item_view.h"

#include "base/common/datetime.h"

namespace nlive {

namespace project_widget {

AudioStorageItemView::AudioStorageItemView(QWidget* parent, sptr<AudioStorageItem> item, sptr<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {
  sptr<AudioStorageItem> asi = std::static_pointer_cast<AudioStorageItem>(item);
  QString dur = QString::fromStdString(datetime::secondsToHHmmss(asi->audio_resource()->duration_in_seconds()));
  title_view()->left_label_box()->setText(item->name());
  title_view()->right_label_box()->setText(dur);
}

AudioStorageItemViewFactory::AudioStorageItemViewFactory(sptr<ServiceLocator> service_locator) :
  StorageItemViewFactory(service_locator) {}
StorageItemView* AudioStorageItemViewFactory::create(QWidget* parent, sptr<StorageItem> item) {
  Q_ASSERT(item->type() == AudioStorageItem::TYPE);
  sptr<AudioStorageItem> vrsi = std::static_pointer_cast<AudioStorageItem>(item);
  return new AudioStorageItemView(parent, vrsi, service_locator_->getService<IThemeService>(IThemeService::ID));
}

}

}