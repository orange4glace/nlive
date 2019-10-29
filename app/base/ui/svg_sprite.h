#ifndef NLIVE_SVG_SPRITE_H_
#define NLIVE_SVG_SPRITE_H_

#include <QPixmap>
#include <QString>

namespace nlive {

class SvgSprite {

private:
  QString path_;
  int width_;
  int height_;
  QPixmap* pixmap_;

  void doCreateSprite();

public:
  SvgSprite(QString path, int width, int height);
  ~SvgSprite();

  inline void resize(int width, int height) {
    if (width_ == width && height_ == height) return;
    width_ = width;
    height_ = height;
    doCreateSprite();
  }

  inline int width() { return width_; }
  inline int height() { return height_; }
  
  inline const QPixmap &pixmap() {
    return *pixmap_;
  }

};

}

#endif