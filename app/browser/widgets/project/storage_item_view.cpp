#include "browser/widgets/project/storage_item_view.h"

#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QTimer>
#include <QDebug>

#include "model/storage/storage_item.h"
#include "browser/drag/static_drag_and_drop.h"

namespace nlive {

namespace project_widget {

StorageItemView::StorageItemView(QWidget* parent, StorageItem* item, QSharedPointer<IThemeService> theme_service) :
  theme_service_(theme_service), QWidget(parent), storage_item_(item) {

}

void StorageItemView::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    QDrag* drag = new QDrag(this);
    qDebug() << "Drag start\n";
    QMimeData* mime_data = new QMimeData;
    mime_data->setText("StorageItemView");
    drag->setMimeData(mime_data);
    StaticDragAndDrop::setData(drag, this);
    drag->exec();
  }
}

StorageItem* StorageItemView::storage_item() {
  return storage_item_;
}

}

}