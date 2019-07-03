#ifndef _NLIVE_LAYOUT_FILL_PARENT_VIEW_H_
#define _NLIVE_LAYOUT_FILL_PARENT_VIEW_H_

#include <QWidget>

namespace nlive {

class FillParentView : public QWidget {
  Q_OBJECT

private:
  QWidget* content_;

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  FillParentView(QWidget* parent, QWidget* widget = nullptr);
  QWidget* setContent(QWidget* widget);

};

}

#endif