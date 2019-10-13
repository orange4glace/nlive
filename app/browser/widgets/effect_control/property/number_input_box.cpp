#include "browser/widgets/effect_control/property/number_input_box.h"

#include <QPainter>
#include <QDebug>
#include <QString>
#include <QMouseEvent>

namespace nlive {

namespace effect_control {


NumberInputBox::EditView::EditView(NumberInputBox* parent, double value) :
  QLineEdit(parent), number_input_box_(parent), value_(value) {
  setAlignment(Qt::AlignCenter);
  setValue(value);

  connect(this, &NumberInputBox::EditView::textEdited, [this](const QString& text) {
    number_input_box_->setValue(text.toDouble());
  });
}

void NumberInputBox::EditView::setValue(double value) {
  QString value_text = QString::number(value);
  setText(value_text);
}

void NumberInputBox::EditView::focusOutEvent(QFocusEvent *e) {
  number_input_box_->switchToSlideView();
}


NumberInputBox::SlideView::SlideView(NumberInputBox* parent, double value) :
  QWidget(parent), number_input_box_(parent), value_(value) {

}

void NumberInputBox::SlideView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), Qt::darkRed);
  p.drawText(rect(), Qt::AlignCenter, QString::number(value_));
}

void NumberInputBox::SlideView::mousePressEvent(QMouseEvent* event) {
  last_mouse_pos_ = event->pos();
  press_and_moved_ = false;
}

void NumberInputBox::SlideView::mouseMoveEvent(QMouseEvent* event) {
  QPoint dp = event->pos() - last_mouse_pos_;
  number_input_box_->setValue(number_input_box_->value() + dp.x());
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
  update();
}




NumberInputBox::NumberInputBox(QWidget* parent, double value) :
  QWidget(parent), value_(value), handling_(false),
  edit_view_(nullptr), slide_view_(nullptr) {

  switchToSlideView();
}

void NumberInputBox::resizeEvent(QResizeEvent* e) {
  if (edit_view_) edit_view_->resize(size());
  if (slide_view_) slide_view_->resize(size());
}

void NumberInputBox::switchToSlideView() {
  if (edit_view_) {
    delete edit_view_;
    edit_view_ = nullptr;
  }
  if (slide_view_) return;
  slide_view_ = new SlideView(this, value_);
  slide_view_->resize(size());
  slide_view_->show();
  slide_view_->onDidClick.connect(
    sig2_t<void ()>::slot_type
    ([this]() { this->switchToEditView(); }).track(__sig_scope_));
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
  if (edit_view_) edit_view_->setValue(value);
  if (slide_view_) slide_view_->setValue(value);
  update();
  onDidChangeValue(value_);
}

void NumberInputBox::doChangeValue(double value) {
  // doSetValue(value);
}

void NumberInputBox::setValue(double value) {
  doSetValue(value);
}

double NumberInputBox::value() const {
  return value_;
}

}

}