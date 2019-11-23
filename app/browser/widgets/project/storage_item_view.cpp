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

ScrubBar::ScrubBar(QWidget* parent, sptr<IThemeService> theme_service) : QWidget(parent), theme_service_(theme_service) {
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




StorageItemContentViewContainer::StorageItemContentViewContainer(QWidget* parent, sptr<IThemeService> theme_service) :
  Div(parent) {
  content_view_ = nullptr;
  scrub_bar_ = new ScrubBar(this, theme_service);
  scrub_bar_->hide();
}

void StorageItemContentViewContainer::doLayout() {
  if (content_view_) content_view_->setGeometry(rect());
  scrub_bar_->setGeometry(0, height() - 3, width(), 3);
}

void StorageItemContentViewContainer::_onScrubStart() {
  if (content_view_) content_view_->onScrubStart();
  scrub_bar_->show();
}

void StorageItemContentViewContainer::_onScrub(double x) {
  scrub_bar_->setValue(x);
  if (content_view_) content_view_->onScrub(x);
}

void StorageItemContentViewContainer::_onScrubEnd() {
  scrub_bar_->hide();
  if (content_view_) content_view_->onScrubEnd();
}

void StorageItemContentView::onScrubStart() {}
void StorageItemContentView::onScrub(double x) {}
void StorageItemContentView::onScrubEnd() {}

void StorageItemContentViewContainer::enterEvent(QEvent* event) {
  scrubbing_ = true;
  setMouseTracking(true);
  _onScrubStart();
}

void StorageItemContentViewContainer::mouseMoveEvent(QMouseEvent* event) {
  double x = event->localPos().x() / (double)width();
  _onScrub(x);
}

void StorageItemContentViewContainer::leaveEvent(QEvent* event) {
  scrubbing_ = false;
  setMouseTracking(false);
  _onScrubEnd();
}

void StorageItemContentViewContainer::contentRectUpdated() {
  doLayout();
}

void StorageItemContentViewContainer::setContentView(StorageItemContentView* content_view) {
  content_view_ = content_view;
  content_view_->setParent(this);
  content_view_->show();
  doLayout();
}







StorageItemTitleView::StorageItemTitleView(QWidget* parent, sptr<IThemeService> theme_service) :
  FlexLayout(parent), theme_service_(theme_service) {
  left_label_box_ = new TextBox(this, "", Qt::AlignBottom);
  right_label_box_ = new TextBox(this, "", Qt::AlignBottom | Qt::AlignRight);
  this->addChild(left_label_box_);
  this->addChild(right_label_box_);
  auto& theme = theme_service->getTheme();
  left_label_box_->setColor(theme.surfaceTextColor());
  right_label_box_->setColor(theme.surfaceTextColor());

  theme_service->onDidUpdate.connect(SIG2_TRACK(sig2_t<void()>::slot_type(
    [this]() {
      auto& theme = theme_service_->getTheme();
      left_label_box_->setColor(theme.surfaceTextColor());
      right_label_box_->setColor(theme.surfaceTextColor());
    }
  )));
}






StorageItemView::StorageItemView(QWidget* parent, sptr<StorageItem> item, sptr<IThemeService> theme_service) :
  Div(parent), theme_service_(theme_service), storage_item_(item) {
  content_view_ = new StorageItemContentViewContainer(this, theme_service);
  title_view_ = new StorageItemTitleView(this, theme_service);
  setPadding(Div::ALL, 5);
}

void StorageItemView::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    QDrag* drag = new QDrag(this);
    QMimeData* mime_data = new QMimeData;
    mime_data->setText("StorageItemView");
    drag->setMimeData(mime_data);
    StaticDragAndDrop::setData(drag, this);
    drag->exec();
  }
}

void StorageItemView::mouseDoubleClickEvent(QMouseEvent* e) {
  onMouseDoubleClick();
}

void StorageItemView::contentRectUpdated() {
  auto cr = rect();
  cr.setHeight(height() - 20);
  auto tr = rect();
  tr.setTop(height() - 20);
  content_view_->setGeometry(cr);
  title_view_->setGeometry(tr);
}

sptr<StorageItem> StorageItemView::storage_item() {
  return storage_item_;
}

}

}