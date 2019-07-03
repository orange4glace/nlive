#include "browser/main_window.h"

#include "platform/theme/themeservice-impl.h"
#include "model/sequence/sequence.h"
#include "model/sequence/track.h"
#include "model/sequence/clip.h"

#include "browser/widgets/timeline/timelinewidget.h"
#include "browser/widgets/project/project_widget.h"

namespace nlive {

MainWindow::MainWindow() {

  // Initialize services
  ThemeService::Initialize();

  auto theme_service = ThemeService::instance();

  // Create Sequence mock data
  auto sequence = new nlive::Sequence();
  sequence->setDuration(500);
  sequence->addTrack();
  auto track1 = sequence->addTrack();
  auto track2 = sequence->addTrack();
  track1->addClip(new nlive::Clip(30, 150));
  track2->addClip(new nlive::Clip(120, 260));

  auto timeline_widget = new timelinewidget::TimelineWidget(nullptr, theme_service);
  timeline_widget->setSequence(sequence);
  addDockWidget(Qt::BottomDockWidgetArea, timeline_widget);

  auto project_widget = new project_widget::ProjectWidget(nullptr);
  addDockWidget(Qt::TopDockWidgetArea, project_widget);

}

}