#include "base/layout/fillparentview.h"

#include <QResizeEvent>
#include <QPainter>
#include <QDebug>

namespace nlive {

FillParentView::FillParentView(QWidget* parent, QWidget* widget) :
  QWidget(parent),
  content_(nullptr) {
  setContent(widget);
}

QWidget* FillParentView::setContent(QWidget* widget) {
  QWidget* old = content_;
  content_ = widget;
  if (widget) widget->setParent(this);
  return old;
}

void FillParentView::resizeEvent(QResizeEvent* event) {
  if (content_) content_->resize(width(), height());
  QWidget::resizeEvent(event);
}

}