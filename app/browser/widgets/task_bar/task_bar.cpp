#include "browser/widgets/task_bar/task_bar.h"

#include <QDebug>
#include <QPainter>

namespace nlive {

namespace task_bar {

TaskBar::TaskBar(QWidget* parent, sptr<ITaskService> task_service,
    sptr<IThemeService> theme_service) :
  Div(parent), task_service_(task_service), theme_service_(theme_service) {
  first_task_ = nullptr;
  progress_bar_ = nullptr;
  setPadding(Div::ALL, 2);
  updateProgressBar();
  task_service_->onDidAddTask.connect(SIG2_TRACK(sig2_t<void (sptr<Task>)>::slot_type(
    [this](sptr<Task> task) {
      updateProgressBar();
    })));
  task_service_->onDidRemoveTask.connect(SIG2_TRACK(sig2_t<void (sptr<Task>)>::slot_type(
    [this](sptr<Task> task) {
      updateProgressBar();
    })));
}

void TaskBar::doLayout() {
  if (progress_bar_) {
    QRect r = rect();
    r.setLeft(r.right() - 100);
    progress_bar_->setGeometry(r);
  }
}

void TaskBar::updateProgressBar() {
  auto& tasks = task_service_->tasks();
  qDebug() << "[TaskBar] updateProgressBar" << tasks.size();
  if (tasks.size() == 0) {
    if (first_task_) {
      first_task_ = nullptr;
      delete progress_bar_;
      progress_bar_ = nullptr;
    }
  }
  else {
    if (first_task_ != tasks[0]) {
      if (first_task_) {
        first_task_ = nullptr;
        delete progress_bar_;
        progress_bar_ = nullptr;
      }
      first_task_ = tasks[0];
      progress_bar_ = new ProgressBar(this, first_task_, theme_service_);
      progress_bar_->show();
    }
  }
  doLayout();
}

void TaskBar::paintEvent(QPaintEvent* e) {
  QPainter p(this);
}

void TaskBar::contentRectUpdated() {
  doLayout();
}

}

}