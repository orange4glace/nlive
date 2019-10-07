#ifndef NLIVE_EFFECT_CONTROL_NUMBER_INPUT_BOX_H_
#define NLIVE_EFFECT_CONTROL_NUMBER_INPUT_BOX_H_

#include <QWidget>
#include <QLineEdit>
#include "base/common/sig.h"

namespace nlive {

namespace effect_control {

class NumberInputBox : public QWidget, protected Sig {
  Q_OBJECT

private:
  class EditView : public QLineEdit {
  
  private:
    double value_;

  public:
    EditView(QWidget* parent, double value);


  };

  class SlideView : public QWidget, protected Sig {

  private:
    double value_;
    QPoint last_mouse_pos_;
    bool press_and_moved_;

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

  public:
    SlideView(QWidget* parent, double value);
    void setValue(double value);

    sig2_t<void (double)> onDidChangeValue;
    sig2_t<void ()> onDidClick; 

  };

  double value_;
  bool handling_;
  
  NumberInputBox::EditView* edit_view_;
  NumberInputBox::SlideView* slide_view_;

  void switchToSlideView();
  void switchToEditView();

  void doSetValue(double value);
  void doChangeValue(double value);

public:
  NumberInputBox(QWidget* parent, double value);
  void setValue(double value);

signals:
  void onDidChangeValue(double value);

};

}

}

#endif