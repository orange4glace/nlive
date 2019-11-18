#include "browser/widgets/project/storage_item_view/sequence_storage_item_view.h"

#include "base/common/datetime.h"

namespace nlive {

namespace project_widget {

SequenceStorageItemView::SequenceStorageItemView(QWidget* parent, QSharedPointer<SequenceStorageItem> item, QSharedPointer<IThemeService> theme_service) :
  StorageItemView(parent, item, theme_service) {
  QSharedPointer<SequenceStorageItem> asi = item.staticCast<SequenceStorageItem>();
  auto sequence = asi->sequence();
  QString dur = QString::fromStdString(datetime::secondsToHHmmss(sequence->duration_in_seconds()));
  title_view()->left_label_box()->setText(item->name());
  title_view()->right_label_box()->setText(dur);
}

SequenceStorageItemViewFactory::SequenceStorageItemViewFactory() {}
StorageItemView* SequenceStorageItemViewFactory::create(QWidget* parent, QSharedPointer<StorageItem> item, QSharedPointer<IThemeService> theme_service) {
  Q_ASSERT(item->type() == SequenceStorageItem::TYPE);
  QSharedPointer<SequenceStorageItem> vrsi = item.staticCast<SequenceStorageItem>();
  return new SequenceStorageItemView(parent, vrsi, theme_service);
}

}

}