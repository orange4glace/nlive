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

ScrubBar::ScrubBar(QWidget* parent, QSharedPointer<IThemeService> theme_service) : QWidget(parent), theme_service_(theme_service) {
  value_ = 0;
}

void ScrubBar::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  p.fillRect(rect(), theme_service_->getTheme().primaryColor());
  p.fillRect(width() * value_ - 3, 0, 6, height(), theme_service_->getTheme().surfaceColor());
}

void ScrubBar::setValue(double x) {
  value_ = x;
  update();
}

StorageItemView::StorageItemView(QWidget* parent, QSharedPointer<StorageItem> item, QSharedPointer<IThemeService> theme_service) :
  Div(parent), theme_service_(theme_service), storage_item_(item) {
  scrub_bar_ = new ScrubBar(this, theme_service);
  scrub_bar_->hide();
  setPadding(Div::ALL, 5);
}

void StorageItemView::_onScrubStart() {
  onScrubStart();
  scrub_bar_->show();
}

void StorageItemView::_onScrub(double x) {
  scrub_bar_->setValue(x);
  onScrub(x);
}

void StorageItemView::_onScrubEnd() {
  scrub_bar_->hide();
  onScrubEnd();
}

void StorageItemView::onScrubStart() {}
void StorageItemView::onScrub(double x) {}
void StorageItemView::onScrubEnd() {}

void StorageItemView::contentRectUpdated() {
  setChildGeometry(scrub_bar_, 0, height() - 3, width(), 3);
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

void StorageItemView::enterEvent(QEvent* event) {
  scrubbing_ = true;
  setMouseTracking(true);
  _onScrubStart();
}

void StorageItemView::mouseMoveEvent(QMouseEvent* event) {
  double x = event->localPos().x() / (double)width();
  _onScrub(x);
}

void StorageItemView::leaveEvent(QEvent* event) {
  scrubbing_ = false;
  setMouseTracking(false);
  _onScrubEnd();
}

QSharedPointer<StorageItem> StorageItemView::storage_item() {
  return storage_item_;
}

}

}