#ifndef NLIVE_SVG_SPRITE_H_
#define NLIVE_SVG_SPRITE_H_

#include <QImage>
#include <QString>

namespace nlive {

class SvgSprite {

private:
  QString path_;
  int width_;
  int height_;
  QImage* image_;

  void doCreateSprite();

public:
  SvgSprite(QString path, int width, int height);
  ~SvgSprite();
  
  inline const QImage &image() {
    return *image_;
  }

};

}

#endif