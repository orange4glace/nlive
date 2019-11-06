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
  sliding_step_ = 1;
  text_color_ = Qt::black;
}

void NumberInputBox::SlideView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.setPen(text_color_);
  p.drawText(rect(), Qt::AlignCenter, QString::number(value_));
}

void NumberInputBox::SlideView::mousePressEvent(QMouseEvent* event) {
  last_mouse_pos_ = event->pos();
  press_and_moved_ = false;
}

void NumberInputBox::SlideView::mouseMoveEvent(QMouseEvent* event) {
  QPoint dp = event->pos() - last_mouse_pos_;
  number_input_box_->setValue(number_input_box_->value() + dp.x() * sliding_step_);
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

  slide_view_ = new SlideView(this, value_);
  edit_view_ = new EditView(this, value_);
  edit_view_->resize(size());
  slide_view_->resize(size());
  slide_view_->onDidClick.connect(
    sig2_t<void ()>::slot_type
    ([this]() { this->switchToEditView(); }).track(__sig_scope_));
  switchToSlideView();
}

void NumberInputBox::resizeEvent(QResizeEvent* e) {
  edit_view_->resize(size());
  slide_view_->resize(size());
}

void NumberInputBox::switchToSlideView() {
  slide_view_->show();
  edit_view_->hide();
}

void NumberInputBox::switchToEditView() {
  slide_view_->hide();
  edit_view_->show();
  edit_view_->setFocus();
}

void NumberInputBox::doSetValue(double value, bool doUpdate) {
  value_ = value;
  edit_view_->setValue(value);
  slide_view_->setValue(value);
  update();
  if (doUpdate) onDidChangeValue(value_);
}

void NumberInputBox::doChangeValue(double value) {
  // doSetValue(value);
}

void NumberInputBox::setValue(double value, bool doUpdate) {
  doSetValue(value, doUpdate);
}

double NumberInputBox::value() const {
  return value_;
}

}

}