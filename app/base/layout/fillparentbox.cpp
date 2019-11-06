#include "base/layout/fillparentbox.h"

#include <QResizeEvent>
#include <QPainter>
#include <QDebug>

namespace nlive {

FillParentBox::FillParentBox(QWidget* parent, QWidget* widget) :
  Div(parent),
  content_(nullptr) {
  setContent(widget);
}

QWidget* FillParentBox::setContent(QWidget* widget) {
  QWidget* old = content_;
  content_ = widget;
  if (widget) widget->setParent(this);
  return old;
}

void FillParentBox::contentRectUpdated() {
  if (content_) content_->setGeometry(rect());
}

}