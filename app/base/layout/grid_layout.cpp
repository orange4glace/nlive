#include "base/layout/grid_layout.h"

#include <QtMath>
#include <QPainter>
#include <QDebug>

namespace nlive {

GridLayoutItem::GridLayoutItem(GridLayout* grid_layout, QWidget* content) :
  QWidget(grid_layout), grid_layout_(grid_layout), content_(content) {
  content->setParent(this);
}

void GridLayoutItem::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.drawRect(0, 0, width() - 1, height() - 1);
}

QWidget* GridLayoutItem::content() {
  return content_;
}

GridLayout::GridLayout(QWidget* parent) :
  QWidget(parent) {

}

void GridLayout::resizeEvent(QResizeEvent* event) {
  int max_width = 200;
  int column_size = qCeil(width() / (qreal)max_width);
  int row_size = items_.size() / column_size + (items_.size() % column_size != 0);
  qreal item_width = width() / (qreal)column_size;
  qreal item_height = item_width * 0.856;
  for (int r = 0; r < row_size; r ++) {
    int y = item_height * r;
    for (int c = 0; c < column_size; c ++) {
      int i = r * column_size + c;
      if (i >= items_.size()) break;
      auto item = items_[i];
      int x = item_width * c;
      item->setGeometry(QRect(x, y, item_width, item_height));
    }
  }
}

size_t GridLayout::addWidget(QWidget* widget) {
  return doAddWidget(widget, items_.size());
}

size_t GridLayout::insertWidget(QWidget* widget, size_t index) {
  return doAddWidget(widget, index);
}

QWidget* GridLayout::getWidgetAt(size_t index) {
  if (index >= items_.size()) return nullptr;
  return items_[index]->content();
}

int GridLayout::getWidgetIndex(QWidget* widget) const {
  // TODO : use std::map for log(N) query?
  int i = 0;
  for (auto item : items_) {
    if (item->content() == widget) return i;
    i++;
  }
  return -1;
}

bool GridLayout::hasWidget(QWidget* widget) const {
  return getWidgetIndex(widget) != -1;
}

size_t GridLayout::doAddWidget(QWidget* widget, size_t index) {
  GridLayoutItem* item = new GridLayoutItem(this, widget);
  index = qMin(index, items_.size());
  items_.insert(items_.begin() + index, item);
  return index;
}

}