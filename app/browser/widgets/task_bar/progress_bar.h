#ifndef NLIVE_TASK_BAR_PROGRESS_BAR_H_
#define NLIVE_TASK_BAR_PROGRESS_BAR_H_

#include <QWidget>
#include <QSharedPointer>
#include "base/common/sig.h"
#include "platform/task/task.h"
#include "platform/theme/themeservice.h"

namespace nlive {

namespace task_bar {

class ProgressBar : public QWidget, public Sig {

private:
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<Task> task_;
  qreal progress_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  ProgressBar(QWidget* parent, QSharedPointer<Task> task,
      QSharedPointer<IThemeService> theme_service);

};

}

}

#endif