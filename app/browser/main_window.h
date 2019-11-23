#ifndef NLIVE_MAIN_WINDOW_H_
#define NLIVE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include "browser/widgets/task_bar/task_bar.h"
#include "browser/services/widgets/widgets_service.h"
#include "platform/service/service_locator.h"

namespace nlive {

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  sptr<ServiceLocator> service_locator_;
  sptr<IWidgetsService> widgets_service_;
  task_bar::TaskBar* task_bar_;

protected:
  void paintEvent(QPaintEvent* event) override;

public:
  MainWindow(sptr<IWidgetsService> widgets_service);

};

}

#endif