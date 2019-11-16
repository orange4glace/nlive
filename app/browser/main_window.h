#ifndef NLIVE_MAIN_WINDOW_H_
#define NLIVE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <QPaintEvent>

namespace nlive {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

protected:
  void paintEvent(QPaintEvent* event) override;

};

}

#endif