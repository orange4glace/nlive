#include "browser/widgets/effect_control/property/number_input_box.h"

#include <QPainter>
#include <QDebug>
#include <QString>
#include <QMouseEvent>

namespace nlive {

namespace effect_control {


NumberInputBox::EditView::EditView(QWidget* parent, double value) :
  QLineEdit(parent), value_(value) {
  QString value_text = QString::number(value);
  setText(value_text);
}


NumberInputBox::SlideView::SlideView(QWidget* parent, double value) :
  QWidget(parent), value_(value) {

}

void NumberInputBox::SlideView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), Qt::red);
}

void NumberInputBox::SlideView::mousePressEvent(QMouseEvent* event) {
  last_mouse_pos_ = event->pos();
  press_and_moved_ = false;
}

void NumberInputBox::SlideView::mouseMoveEvent(QMouseEvent* event) {
  QPoint dp = event->pos() - last_mouse_pos_;
  last_mouse_pos_ = event->pos();
  press_and_moved_ = true;
}

void NumberInputBox::SlideView::mouseReleaseEvent(QMouseEvent* event) {
  if (!press_and_moved_) {
    qDebug() << "onDidClick()\n";
    onDidClick();
  }
}

void NumberInputBox::SlideView::setValue(double value) {
  value_ = value;
}




NumberInputBox::NumberInputBox(QWidget* parent, double value) :
  QWidget(parent), value_(value), handling_(false),
  edit_view_(nullptr), slide_view_(nullptr) {

  switchToSlideView();
}

void NumberInputBox::switchToSlideView() {
  if (edit_view_) {
    delete edit_view_;
    edit_view_ = nullptr;
  }
  if (slide_view_) return;
  slide_view_ = new SlideView(this, value_);
  slide_view_->onDidClick.connect(
    sig2_t<void ()>::slot_type
    ([this]() { this->switchToEditView(); }).track(__sig_scope_));
  slide_view_->show();
}

void NumberInputBox::switchToEditView() {
  if (slide_view_) {
    delete slide_view_;
    slide_view_ = nullptr;
  }
  if (edit_view_) return;
  edit_view_ = new EditView(this, value_);
  edit_view_->resize(size());
  edit_view_->show();
  edit_view_->setFocus();
}

void NumberInputBox::doSetValue(double value) {
  value_ = value;
}

void NumberInputBox::doChangeValue(double value) {
  doSetValue(value);
  emit onDidChangeValue(value_);
}

void NumberInputBox::setValue(double value) {
  doSetValue(value);
}

}

}