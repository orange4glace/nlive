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

StorageItemView::StorageItemView(QWidget* parent, StorageItem* item) :
  QWidget(parent), storage_item_(item) {


  QTimer* t = new QTimer();
  connect(t, &QTimer::timeout, this, [this]() {
    if (col_ == Qt::red) col_ = Qt::blue;
    else col_ = Qt::red;
    repaint();
  });
  t->start(1000);

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

void StorageItemView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), col_);
}

StorageItem* StorageItemView::storage_item() {
  return storage_item_;
}

}

}