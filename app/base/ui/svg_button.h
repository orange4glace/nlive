#ifndef NLIVE_SVG_BUTTON_H_
#define NLIVE_SVG_BUTTON_H_

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include "base/common/sig.h"
#include "base/ui/svg_sprite.h"

namespace nlive {

class SvgButton : public QPushButton, public Sig {
  Q_OBJECT

private:
  SvgSprite svg_;
  bool embossing_;

protected:
  inline void resizeEvent(QResizeEvent* e) override {
    svg_.resize(width(), height());
  }

  inline void paintEvent(QPaintEvent* e) override {
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    p.drawPixmap(rect(), svg_.pixmap());
  }

public:
  inline SvgButton(QWidget* parent, QString path, bool embossing = true) :
    QPushButton(parent), svg_(path, width(), height()) {

  }

};

}

#endif