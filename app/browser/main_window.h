#ifndef _NLIVE_MAIN_WINDOW_H_
#define _NLIVE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <QPaintEvent>

namespace nlive {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

protected:
  void paintEvent(QPaintEvent* event) override;

public slots:
  void ff();

};

}

#endif