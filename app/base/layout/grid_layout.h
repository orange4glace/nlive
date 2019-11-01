#ifndef _NLIVE_GRID_LAYOUT_H_
#define _NLIVE_GRID_LAYOUT_H_

#include <vector>
#include "base/layout/div.h"

namespace nlive {

class GridLayout;
class GridLayoutItem;

class GridLayoutItem : public Div {

private:
  GridLayout* grid_layout_;
  QWidget* content_;

protected:
  void contentRectUpdated() override;

public:
  GridLayoutItem(GridLayout* grid_layout, QWidget* content);
  QWidget* content();

};

class GridLayout : public Div {

private:
  std::vector<GridLayoutItem*> items_;

  size_t doAddWidget(QWidget* widget, size_t index);
  int doRemoveWidget(QWidget* widget);

  void doLayout();

protected:
  void contentRectUpdated() override;

public:
  GridLayout(QWidget* parent = nullptr);
  size_t addWidget(QWidget* widget);
  size_t insertWidget(QWidget* widget, size_t index);
  int removeWidget(QWidget* widget);
  QWidget* getWidgetAt(size_t index);
  int getWidgetIndex(QWidget* widget) const;
  bool hasWidget(QWidget* widget) const;

};

}

#endif