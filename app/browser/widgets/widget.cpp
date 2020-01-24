#include "browser/widgets/widget.h"

#include <QPen>
#include <QPainter>
#include <QDebug>
#include "browser/services/widgets/widgets_service.h"

namespace nlive {

Widget::Widget(QWidget* parent, sptr<IWidgetsService> widgets_service,
    sptr<IThemeService> theme_service) :
    QDockWidget(parent), widgets_service_(widgets_service),
    theme_service_(theme_service){
  setFocusPolicy(Qt::ClickFocus);

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

void Widget::focusInEvent(QFocusEvent* e) {
  focused_ = true;
  widgets_service_->setFocusedWidget(this);
  onFocused();
  onDidFocus();
}

void Widget::focusOutEvent(QFocusEvent* e) {
  focused_ = false;
  widgets_service_->setFocusedWidget(nullptr);
  onBlured();
  onDidBlur();
}

void Widget::onFocused() {}
void Widget::onBlured() {}

bool Widget::focused() const {
  return focused_;
}

}