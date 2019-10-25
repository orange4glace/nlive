#ifndef NLIVE_EFFECT_CONTROL_NUMBER_INPUT_BOX_H_
#define NLIVE_EFFECT_CONTROL_NUMBER_INPUT_BOX_H_

#include <QWidget>
#include <QLineEdit>
#include <QResizeEvent>
#include "base/common/sig.h"

namespace nlive {

namespace effect_control {

class NumberInputBox : public QWidget, protected Sig {
  Q_OBJECT

private:
  class EditView : public QLineEdit {
  
  private:
    NumberInputBox* number_input_box_;
    double value_;

  protected:
    void focusOutEvent(QFocusEvent *e) override;

  public:
    EditView(NumberInputBox* parent, double value);
    void setValue(double value);


  };

  class SlideView : public QWidget, protected Sig {

  private:
    NumberInputBox* number_input_box_;
    double value_;
    QPoint last_mouse_pos_;
    bool press_and_moved_;

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

  public:
    SlideView(NumberInputBox* parent, double value);
    void setValue(double value);

    sig2_t<void (double)> onDidChangeValue;
    sig2_t<void ()> onDidClick; 

  };

  double value_;
  bool handling_;
  
  NumberInputBox::EditView* edit_view_;
  NumberInputBox::SlideView* slide_view_;

  void doSetValue(double value, bool doUpdate);
  void doChangeValue(double value);

protected:
  void resizeEvent(QResizeEvent* e) override;

public:
  NumberInputBox(QWidget* parent, double value);
  void setValue(double value, bool doUpdate = true);

  void switchToSlideView();
  void switchToEditView();

  double value() const;

  sig2_t<void (double)> onDidChangeValue;

};

}

}

#endif