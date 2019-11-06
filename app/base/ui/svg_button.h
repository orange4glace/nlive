#ifndef NLIVE_SVG_BUTTON_H_
#define NLIVE_SVG_BUTTON_H_

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include "base/layout/div.h"
#include "base/ui/svg_sprite.h"

namespace nlive {

class SvgButton : public Div {
  Q_OBJECT

private:
  SvgSprite svg_;
  bool embossing_;

  bool checked_;
  
  bool pressed_;

protected:
  inline void mousePressEvent(QMouseEvent* e) {
    pressed_ = true;
  }

  inline void mouseReleaseEvent(QMouseEvent* e) {
    if (rect().contains(e->pos())) {
      checked_ = !checked_;
      clicked(checked_);
    }
    pressed_ = false;
  }

  inline void contentRectUpdated() override {
    svg_.resize(width(), height());
    update();
  }

  inline void paintEvent(QPaintEvent* e) override {
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    p.drawPixmap(rect(), svg_.pixmap());
  }

public:
  inline SvgButton(QWidget* parent, QString path, bool embossing = true) :
    Div(parent), svg_(path, width(), height()) {
    checked_ = pressed_ = false;
    embossing_ = embossing;
  }

  inline bool checked() const { return checked_; }
  inline bool pressed() const { return pressed_; }

  sig2_t<void (bool /*checked*/)> clicked;

};

}

#endif