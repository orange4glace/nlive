#ifndef NLIVE_SPRITE_BUTTON_H_
#define NLIVE_SPRITE_BUTTON_H_

#include <QPushButton>
#include <QPixmap>
#include "base/common/sig.h"

namespace nlive {

class SpriteButton : public QPushButton, public Sig {
  Q_OBJECT

private:
  QPixmap pixmap_;
  bool embossing_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  SpriteButton(QWidget* parent, QPixmap pixmap, bool embossing);

};

}

#endif