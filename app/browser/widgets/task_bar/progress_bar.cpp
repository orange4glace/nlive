#include "browser/widgets/task_bar/progress_bar.h"

#include <QDebug>
#include <QPainter>

namespace nlive {

namespace task_bar {

ProgressBar::ProgressBar(QWidget* parent, QSharedPointer<Task> task,
    QSharedPointer<IThemeService> theme_service) :
  QWidget(parent), theme_service_(theme_service), task_(task), progress_(0) {
  
  qDebug() << "Hello Progressbar";
  task_->onUpdateProgress.connect(SIG2_TRACK(sig2_t<void (qreal)>::slot_type(
    [this](qreal value) {
      progress_ = value;
      update();
  })));

}

void ProgressBar::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  auto theme = theme_service_->getTheme();
  p.setRenderHint(QPainter::Antialiasing);
  QPainterPath path1;
  QRect r = rect();
  r.setWidth(r.width() * progress_);
  path1.addRoundedRect(r, 2, 2);
  QPainterPath path2;
  path2.addRoundedRect(rect(), 2, 2);
  p.setPen(theme.surfaceBrightColor());
  p.fillPath(path1, theme.primaryColor());
  p.drawPath(path2);
}

}
  
}