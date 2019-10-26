#include "base/ui/sprite_button.h"

#include <QPainter>

namespace nlive {

SpriteButton::SpriteButton(QWidget* parent, QPixmap pixmap, bool embossing) :
  QPushButton(parent), pixmap_(pixmap), embossing_(embossing) {

}

void SpriteButton::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  p.drawPixmap(rect(), pixmap_);
}

}