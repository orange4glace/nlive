#ifndef NLIVE_TEXT_BOX_H_
#define NLIVE_TEXT_BOX_H_

#include <QWidget>
#include <QString>
#include <QPainter>
#include "base/layout/div.h"

namespace nlive {

class TextBox : public Div {

private:
  QString text_;
  QColor color_;
  int flag_;

protected:
  inline void paintEvent(QPaintEvent* e) override {
    QPainter p(this);
    QRect crect = contentRect();
    p.setPen(color_);
    p.drawText(crect, flag_, text_);
  }

public:
  inline TextBox(QWidget* parent, const QString& text, int flag = 0) :
    Div(parent), text_(text), flag_(flag) {
    color_ = Qt::black;
  }

  inline TextBox* setText(const QString& text) {
    text_ = text;
    update();
    return this;
  }

  inline TextBox* setColor(const QColor& color) {
    color_ = color;
    update();
    return this;
  }

  inline TextBox* setFlag(int flag) {
    flag_ = flag;
    return this;
  }

  inline const QString& text() const { return text_; }
  inline int flag() const { return flag_; }

};

}

#endif