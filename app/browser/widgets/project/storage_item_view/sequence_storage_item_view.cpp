#include "browser/widgets/project/storage_item_view/sequence_storage_item_view.h"

#include <QDebug>
#include "base/common/datetime.h"
#include "browser/widgets/timeline/timeline_widget.h"

namespace nlive {

namespace project_widget {

SequenceStorageItemView::SequenceStorageItemView(QWidget* parent, QSharedPointer<SequenceStorageItem> item,
    QSharedPointer<IThemeService> theme_service, QSharedPointer<IWidgetsService> widgets_service) :
  StorageItemView(parent, item, theme_service), widgets_service_(widgets_service),
  item_(item) {
  QSharedPointer<SequenceStorageItem> asi = item.staticCast<SequenceStorageItem>();
  auto sequence = asi->sequence();
  QString dur = QString::fromStdString(datetime::secondsToHHmmss(sequence->duration_in_seconds()));
  title_view()->left_label_box()->setText(item->name());
  title_view()->right_label_box()->setText(dur);
}

void SequenceStorageItemView::onMouseDoubleClick() {
  auto widget = widgets_service_->getWidget(timeline_widget::TimelineWidget::TYPE);
  qDebug() << "DBCL" << widget;
  if (!widget) return;
  auto timeline_widget = static_cast<timeline_widget::TimelineWidget*>(widget);
  timeline_widget->setSequence(item_->sequence());
}

SequenceStorageItemViewFactory::SequenceStorageItemViewFactory(QSharedPointer<ServiceLocator> service_locator) :
  StorageItemViewFactory(service_locator) {}
StorageItemView* SequenceStorageItemViewFactory::create(QWidget* parent, QSharedPointer<StorageItem> item) {
  Q_ASSERT(item->type() == SequenceStorageItem::TYPE);
  QSharedPointer<SequenceStorageItem> vrsi = item.staticCast<SequenceStorageItem>();
  return new SequenceStorageItemView(parent, vrsi,
      service_locator_->getService<IThemeService>(IThemeService::ID),
      service_locator_->getService<IWidgetsService>(IWidgetsService::ID));
}

}

}