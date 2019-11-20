#ifndef NLIVE_MAIN_WINDOW_H_
#define NLIVE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include "browser/widgets/task_bar/task_bar.h"

namespace nlive {

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  task_bar::TaskBar* task_bar_;

public:
  MainWindow();

protected:
  void paintEvent(QPaintEvent* event) override;

};

}

#endif