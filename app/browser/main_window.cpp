#include "browser/main_window.h"

#include "base/layout/grid_layout.h"

#include "platform/theme/themeservice-impl.h"
#include "platform/task/task_service_impl.h"
#include "platform/resource/resource_service_impl.h"
#include "model/sequence/sequence.h"
#include "model/sequence/track.h"
#include "model/sequence/clip.h"
#include "model/storage/sequence_storage_item.h"

#include "model/project/project.h"
#include "model/effect/transform_effect.h"

#include "browser/widgets/timeline/timeline_widget.h"
#include "browser/widgets/timeline/timeline_widget_service_impl.h"
#include "browser/widgets/project/project_widget.h"
#include "browser/widgets/monitor/monitor_widget.h"
#include "browser/widgets/effect_control/effect_control_widget.h"
#include "browser/widgets/task_bar/task_bar.h"
#include "browser/services/import/import_service_impl.h"
#include "browser/services/memento/in_memory_memento_service.h"
#include "browser/services/play/play_service.h"
#include "browser/services/projects/projects_service_impl.h"
#include "browser/services/audio_flaty/audio_flaty_service.h"
#include "browser/services/menu_bar/menu_bar_service.h"

#include "platform/task/task.h"
#include "platform/logger/logger.h"

#include "renderer/audio_renderer/test_renderer.h"
#include "renderer/audio_renderer/sequence_renderer.h"

#include <QDebug>
#include <QPainter>
#include <QLayout>
#include <QDialog>
#include <QTimer>
#include <QThread>
#include "base/common/memory.h"
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QFuture>
#include <QStatusBar>
#include <QThreadPool>

namespace nlive {

MainWindow::MainWindow(sptr<IWidgetsService> widgets_service) :
  QMainWindow(), widgets_service_(widgets_service) {

  qDebug() << "Main thread = " << thread() << "\n";

  // auto at = new audio_renderer::TestRenderer(this);

  registerLoggers();

  service_locator_ = sptr<ServiceLocator>(
    new ServiceLocator());

  ThemeService::Initialize();
  TimelineWidgetService::Initialize();
  auto task_service = new sptr<ITaskService>(new TaskService());
  auto theme_service = ThemeService::instance();
  auto projects_service = sptr<IProjectsService>(new ProjectsService());
  auto timeline_widget_service = TimelineWidgetService::instance();
  auto resource_service = new ResourceService(*task_service);
  auto import_service = new ImportService(resource_service);
  auto memento_service = new sptr<IMementoService>(new InMemoryMementoService());
  auto play_service = new sptr<PlayService>(new PlayService(this));
  auto audio_flaty_service = sptr<AudioFlatyService>(new AudioFlatyService(projects_service, *task_service));
  auto menu_bar_service = sptr<MenuBarService>(new MenuBarService(menuBar(), theme_service));
  menu_bar_service->addMenu("file", "&File");

  auto s_resource_service = new sptr<ResourceService>(resource_service);
  auto s_import_service = new sptr<ImportService>(import_service);

  service_locator_->registerService(theme_service);
  service_locator_->registerService(widgets_service);
  service_locator_->registerService(projects_service);
  service_locator_->registerService(audio_flaty_service);
  service_locator_->registerService(menu_bar_service);

  // task_service->setParent(this);
  resource_service->setParent(this);

  QStatusBar* status_bar = new QStatusBar(this);
  auto t = new task_bar::TaskBar(this, *task_service, ThemeService::instance());
  status_bar->addPermanentWidget(t, 1000);
  setStatusBar(status_bar);
  status_bar->show();
  t->show();

  // Create Sequence mock data
  auto project = projects_service->createProject();

  auto root_storage = project->root_storage_directory();
  auto sequence = sptr<Sequence>(new Sequence(project->undo_stack(), 30, 48000));
  auto sequence_storage_item = sptr<SequenceStorageItem>(
    new SequenceStorageItem(root_storage, "sequence", sequence));
  root_storage->addItem(sequence_storage_item);

  sequence->setDuration(500);
  sequence->addTrack();
  auto track1 = sequence->addTrack();
  auto track2 = sequence->addTrack();

  project_widget::ProjectWidget::Initialize(service_locator_);
  auto project_widget = new project_widget::ProjectWidget(nullptr, theme_service, *s_import_service, service_locator_);
  addDockWidget(Qt::BottomDockWidgetArea, project_widget);
  widgets_service->addWidget(project_widget);
  project_widget->setDirectory(project->root_storage_directory());

  auto timeline_widget = new timeline_widget::TimelineWidget(nullptr, theme_service, timeline_widget_service, *play_service);
  // timeline_widget->setSequence(sequence);
  addDockWidget(Qt::BottomDockWidgetArea, timeline_widget);
  widgets_service->addWidget(timeline_widget);

  effect_control::EffectControlWidget::Initialize();
  auto effect_control_widget = new effect_control::EffectControlWidget(
    this, theme_service, timeline_widget_service, *memento_service);
  addDockWidget(Qt::TopDockWidgetArea, effect_control_widget);
  widgets_service->addWidget(effect_control_widget);

  auto monitor_widget = new monitor_widget::MonitorWidget(nullptr, timeline_widget_service, theme_service, *play_service);
  addDockWidget(Qt::TopDockWidgetArea, monitor_widget);
  widgets_service->addWidget(monitor_widget);
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), QColor("#212624"));
}

}