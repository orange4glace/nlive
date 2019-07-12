#include "browser/main_window.h"

#include "base/layout/grid_layout.h"

#include "platform/theme/themeservice-impl.h"
#include "platform/task/task_service_impl.h"
#include "platform/resource/resource_service_impl.h"
#include "model/sequence/sequence.h"
#include "model/sequence/track.h"
#include "model/sequence/clip.h"

#include "model/project/project.h"

#include "browser/widgets/timeline/timelinewidget.h"
#include "browser/widgets/project/project_widget.h"
#include "browser/services/import/import_service_impl.h"

#include "platform/task/task.h"

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

  // Initialize services
  ThemeService::Initialize();

  auto theme_service = ThemeService::instance();
  auto task_service = new TaskService();
  auto resource_service = new ResourceService(task_service);
  auto import_service = new ImportService(resource_service);

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
  track1->addClip(QSharedPointer<Clip>(new nlive::Clip(track1->undo_stack(), sequence->timebase(), 30, 150, 0)));
  track2->addClip(QSharedPointer<Clip>(new nlive::Clip(track2->undo_stack(), sequence->timebase(), 120, 260, 0)));

  auto timeline_widget = new timelinewidget::TimelineWidget(nullptr, theme_service);
  timeline_widget->setSequence(sequence);
  addDockWidget(Qt::BottomDockWidgetArea, timeline_widget);

  project_widget::ProjectWidget::Initialize();
  auto project_widget = new project_widget::ProjectWidget(nullptr, theme_service, import_service);
  addDockWidget(Qt::TopDockWidgetArea, project_widget);

  project_widget->setDirectory(project->root_storage_directory());

}

void MainWindow::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), QColor("#212624"));
}

void MainWindow::ff() {
  qDebug() << "ff = " << QThread::currentThreadId() << "\n";
}

}