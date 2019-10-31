#ifndef NLIVE_SOLID_BOX_H_
#define NLIVE_SOLID_BOX_H_

#include <QPainter>
#include "base/layout/div.h"

namespace nlive {

class SolidBox : public Div {

private:
  QColor color_;

protected:
  inline void paintEvent(QPaintEvent* e) {
    QPainter p(this);
    p.fillRect(rect(), color_);
  }

public:
  inline SolidBox(QWidget* parent, QColor color = Qt::blue) :
    Div(parent), color_(color) {

  }

};

}

#endif