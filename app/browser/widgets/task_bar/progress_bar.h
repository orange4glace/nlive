#ifndef NLIVE_TASK_BAR_PROGRESS_BAR_H_
#define NLIVE_TASK_BAR_PROGRESS_BAR_H_

#include <QWidget>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "platform/task/task.h"
#include "platform/theme/themeservice.h"

namespace nlive {

namespace task_bar {

class ProgressBar : public QWidget, public Sig {

private:
  sptr<IThemeService> theme_service_;

  sptr<Task> task_;
  qreal progress_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  ProgressBar(QWidget* parent, sptr<Task> task,
      sptr<IThemeService> theme_service);

};

}

}

#endif