#include "browser/widgets/effect_control/property/property_view.h"

#include <QPainter>

namespace nlive {

namespace effect_control {

PropertyLabelView::PropertyLabelView(
  QWidget* parent, QString label, int flags) :
  QWidget(parent), label_(label), flags_(flags) {

}

void PropertyLabelView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.setRenderHint(QPainter::RenderHint::Antialiasing);
  p.drawText(rect(), flags_, label_);
}

int PropertyLabelView::flags() const {
  return flags_;
}

}

}