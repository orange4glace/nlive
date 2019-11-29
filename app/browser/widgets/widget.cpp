#include "browser/widgets/widget.h"

#include <QPen>
#include <QPainter>

namespace nlive {

Widget::Widget(QWidget* parent, sptr<IThemeService> theme_service) :
  QDockWidget(parent), theme_service_(theme_service) {

}

void Widget::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  auto theme = theme_service_->getTheme();
  QPen pen;
  pen.setColor(theme.surfaceBrightColor());
  pen.setWidth(1);
  p.setPen(pen);
  p.drawRect(rect().x(), rect().y(), rect().width() - 1, rect().height() - 1);
}


}