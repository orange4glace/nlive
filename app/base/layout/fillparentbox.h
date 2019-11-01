#ifndef NLIVE_LAYOUT_FILL_PARENT_BOX_H_
#define NLIVE_LAYOUT_FILL_PARENT_BOX_H_

#include <QWidget>
#include "base/layout/div.h"

namespace nlive {

class FillParentBox : public Div {
  Q_OBJECT

private:
  QWidget* content_;

protected:
  void contentRectUpdated() override;

public:
  FillParentBox(QWidget* parent, QWidget* widget = nullptr);
  QWidget* setContent(QWidget* widget);

};

}

#endif