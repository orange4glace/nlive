#ifndef _NLIVE_GRID_LAYOUT_H_
#define _NLIVE_GRID_LAYOUT_H_

#include <QWidget>
#include <vector>

namespace nlive {

class GridLayout;
class GridLayoutItem;

class GridLayoutItem : public QWidget {

private:
  GridLayout* grid_layout_;
  QWidget* content_;

protected:
  void paintEvent(QPaintEvent* event) override;

public:
  GridLayoutItem(GridLayout* grid_layout, QWidget* content);
  QWidget* content();

};

class GridLayout : public QWidget {

private:
  std::vector<GridLayoutItem*> items_;

  size_t doAddWidget(QWidget* widget, size_t index);

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  GridLayout(QWidget* parent = nullptr);
  size_t addWidget(QWidget* widget);
  size_t insertWidget(QWidget* widget, size_t index);
  QWidget* getWidgetAt(size_t index);
  int getWidgetIndex(QWidget* widget) const;
  bool hasWidget(QWidget* widget) const;

};

}

#endif