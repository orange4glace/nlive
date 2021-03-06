#include "base/layout/grid_layout.h"

#include <QtMath>
#include <QPainter>
#include <QDebug>

namespace nlive {

GridLayoutItem::GridLayoutItem(GridLayout* grid_layout, QWidget* content) :
  Div(grid_layout), grid_layout_(grid_layout), content_(content) {
  content->setParent(this);
  content->show();
}

void GridLayoutItem::contentRectUpdated() {
  content_->setGeometry(contentRect());
}

QWidget* GridLayoutItem::content() {
  return content_;
}

GridLayout::GridLayout(QWidget* parent) :
  Div(parent) {

}

size_t GridLayout::doAddWidget(QWidget* widget, size_t index) {
  GridLayoutItem* item = new GridLayoutItem(this, widget);
  index = qMin(index, items_.size());
  items_.insert(items_.begin() + index, item);
  item->show();
  doLayout();
  return index;
}

int GridLayout::doRemoveWidget(QWidget* widget) {
  int idx = getWidgetIndex(widget);
  if (idx == -1) return -1;
  items_.erase(items_.begin() + idx);
  widget->setParent(nullptr);
  doLayout();
  return idx;
}

void GridLayout::doLayout() {
  const QRect& crect = content_rect();
  int max_width = 200;
  int column_size = qCeil(crect.width() / (qreal)max_width);
  int row_size = items_.size() / column_size + (items_.size() % column_size != 0);
  qreal item_width = crect.width() / (qreal)column_size;
  qreal item_height = item_width * 0.856;
  for (int r = 0; r < row_size; r ++) {
    int y = item_height * r + crect.y();
    for (int c = 0; c < column_size; c ++) {
      int i = r * column_size + c;
      if (i >= items_.size()) break;
      auto item = items_[i];
      int x = item_width * c + crect.x();
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

int GridLayout::removeWidget(QWidget* widget) {
  return doRemoveWidget(widget);
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

void GridLayout::contentRectUpdated() {
  doLayout();
}

}