#ifndef NLIVE_WIDGETS_TASK_BAR_H_
#define NLIVE_WIDGETS_TASK_BAR_H_

#include <QWidget>
#include "base/common/memory.h"
#include "base/layout/div.h"
#include "base/common/sig.h"
#include "platform/task/task_service.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/task_bar/progress_bar.h"

namespace nlive {

namespace task_bar {

class TaskBar : public Div {

private:
  sptr<ITaskService> task_service_;
  sptr<IThemeService> theme_service_;

  sptr<Task> first_task_;
  ProgressBar* progress_bar_;

  void updateProgressBar();
  void doLayout();

protected:
  void paintEvent(QPaintEvent* e) override;
  void contentRectUpdated() override;

public:
  TaskBar(QWidget* parent, sptr<ITaskService> task_service,
      sptr<IThemeService> theme_service);

};

}

}

#endif