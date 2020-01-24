#include "browser/widgets/project/storage_item_view/sequence_storage_item_view.h"

#include <QDebug>
#include "base/common/datetime.h"
#include "browser/widgets/timeline/timeline_widget.h"

namespace nlive {

namespace project_widget {

SequenceStorageItemView::SequenceStorageItemView(QWidget* parent, sptr<SequenceStorageItem> item,
    sptr<IThemeService> theme_service, sptr<IWidgetsService> widgets_service) :
  StorageItemView(parent, item, theme_service), widgets_service_(widgets_service),
  item_(item) {
  sptr<SequenceStorageItem> asi = std::static_pointer_cast<SequenceStorageItem>(item);
  auto sequence = asi->sequence();
  QString dur = QString::fromStdString(datetime::secondsToHHmmss(sequence->duration_in_seconds()));
  title_view()->left_label_box()->setText(item->name());
  title_view()->right_label_box()->setText(dur);
}

void SequenceStorageItemView::onMouseDoubleClick() {
  auto widget = widgets_service_->getWidget(timeline_widget::TimelineWidget::ID);
  if (!widget) return;
  auto timeline_widget = static_cast<timeline_widget::TimelineWidget*>(widget);
  timeline_widget->setSequenceStorageItem(item_);
}

SequenceStorageItemViewFactory::SequenceStorageItemViewFactory(sptr<ServiceLocator> service_locator) :
  StorageItemViewFactory(service_locator) {}
StorageItemView* SequenceStorageItemViewFactory::create(QWidget* parent, sptr<StorageItem> item) {
  Q_ASSERT(item->type() == SequenceStorageItem::TYPE);
  sptr<SequenceStorageItem> vrsi = std::static_pointer_cast<SequenceStorageItem>(item);
  return new SequenceStorageItemView(parent, vrsi,
      service_locator_->getService<IThemeService>(IThemeService::ID),
      service_locator_->getService<IWidgetsService>(IWidgetsService::ID));
}

}

}