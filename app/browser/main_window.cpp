#include "browser/main_window.h"

#include "base/layout/grid_layout.h"

#include "platform/theme/themeservice-impl.h"
#include "platform/task/task_service_impl.h"
#include "platform/resource/resource_service_impl.h"
#include "model/sequence/sequence.h"
#include "model/sequence/track.h"
#include "model/sequence/clip.h"

#include "model/project/project.h"
#include "model/effect/transform_effect.h"

#include "browser/widgets/timeline/timelinewidget.h"
#include "browser/widgets/timeline/timeline_widget_service_impl.h"
#include "browser/widgets/project/project_widget.h"
#include "browser/widgets/monitor/monitor_widget.h"
#include "browser/widgets/effect_control/effect_control_widget.h"
#include "browser/services/import/import_service_impl.h"

#include "platform/task/task.h"
#include "platform/logger/logger.h"

#include <QDebug>
#include <QPainter>
#include <QLayout>
#include <QDialog>
#include <QTimer>
#include <QThread>
#include <QSharedPointer>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QFuture>
#include <QThreadPool>

namespace nlive {

namespace {

int qtr(int a) {
  qDebug() << "a = " << a << " " << QThread::currentThreadId() << "\n";
  return a + 3;
}

class KD : public QDockWidget {

public:
  QWidget* q;
  KD(QWidget* p) : QDockWidget(p) {}

protected:
  void resizeEvent(QResizeEvent* event) {
    qDebug() << width() << " " << height() << "\n";
    resize(width(), height());
    q->setGeometry(rect());
  }

};

}

MainWindow::MainWindow() {

  qDebug() << "Main thread = " << thread() << "\n";

  registerLoggers();

  // Initialize services
  ThemeService::Initialize();
  TimelineWidgetService::Initialize();

  auto theme_service = ThemeService::instance();
  auto timeline_widget_service = TimelineWidgetService::instance();
  auto task_service = new TaskService();
  auto resource_service = new ResourceService(task_service);
  auto import_service = new ImportService(resource_service);

  auto s_resource_service = new QSharedPointer<ResourceService>(resource_service);
  auto s_import_service = new QSharedPointer<ImportService>(import_service);

  // task_service->setParent(this);
  resource_service->setParent(this);
  import_service->setParent(this);

  // Create Sequence mock data
  auto project = new Project();
  auto sequence = project->createSequence();
  sequence->setDuration(500);
  sequence->addTrack();
  auto track1 = sequence->addTrack();
  auto track2 = sequence->addTrack();
  auto clip1 = QSharedPointer<Clip>(new nlive::Clip(track1->undo_stack(), sequence->time_base(), 30, 150, 0));
  track1->addClip(clip1);
  track2->addClip(QSharedPointer<Clip>(new nlive::Clip(track2->undo_stack(), sequence->time_base(), 120, 260, 0)));

  QSharedPointer<effect::TransformEffect> transform_effect = QSharedPointer<effect::TransformEffect>(new effect::TransformEffect());
  clip1->addEffect(transform_effect);

  auto timeline_widget = new timelinewidget::TimelineWidget(nullptr, theme_service.get(), timeline_widget_service);
  timeline_widget->setSequence(sequence);
  addDockWidget(Qt::BottomDockWidgetArea, timeline_widget);

  project_widget::ProjectWidget::Initialize();
  auto project_widget = new project_widget::ProjectWidget(nullptr, theme_service, *s_import_service);
  addDockWidget(Qt::TopDockWidgetArea, project_widget);

  project_widget->setDirectory(project->root_storage_directory());

  auto monitor_widget = new monitor_widget::MonitorWidget(nullptr, timeline_widget_service, theme_service.get());
  addDockWidget(Qt::TopDockWidgetArea, monitor_widget);

  // effect_control::EffectControlWidget::Initialize();
  // auto effect_control_widget = new effect_control::EffectControlWidget(
  //   this, theme_service, timeline_widget_service);
  // addDockWidget(Qt::TopDockWidgetArea, effect_control_widget);
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), QColor("#212624"));
}

void MainWindow::ff() {
  qDebug() << "ff = " << QThread::currentThreadId() << "\n";
}

}