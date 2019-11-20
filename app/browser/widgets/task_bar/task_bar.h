#ifndef NLIVE_WIDGETS_TASK_BAR_H_
#define NLIVE_WIDGETS_TASK_BAR_H_

#include <QWidget>
#include <QSharedPointer>
#include "base/layout/div.h"
#include "base/common/sig.h"
#include "platform/task/task_service.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/task_bar/progress_bar.h"

namespace nlive {

namespace task_bar {

class TaskBar : public Div {

private:
  QSharedPointer<ITaskService> task_service_;
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<Task> first_task_;
  ProgressBar* progress_bar_;

  void updateProgressBar();
  void doLayout();

protected:
  void paintEvent(QPaintEvent* e) override;
  void contentRectUpdated() override;

public:
  TaskBar(QWidget* parent, QSharedPointer<ITaskService> task_service,
      QSharedPointer<IThemeService> theme_service);

};

}

}

#endif