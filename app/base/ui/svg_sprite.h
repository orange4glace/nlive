#ifndef NLIVE_SVG_SPRITE_H_
#define NLIVE_SVG_SPRITE_H_

#include <QPixmap>
#include <QString>
#include <QColor>
#include <string>

namespace nlive {

class SvgSprite {

private:
  QString path_;
  int width_;
  int height_;
  std::string color_;
  QPixmap* pixmap_;

  void doCreateSprite();

public:
  SvgSprite(QString path);
  SvgSprite(QString path, int width, int height, std::string color = "black");
  ~SvgSprite();

  void resize(int width, int height);
  void setColor(std::string color);
  void setColor(QColor color);

  inline int width() { return width_; }
  inline int height() { return height_; }
  
  inline const QPixmap &pixmap() {
    return *pixmap_;
  }

};

}

#endif