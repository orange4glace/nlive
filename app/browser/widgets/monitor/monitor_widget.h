#ifndef NLIVE_MONITOR_WIDGET_H_
#define NLIVE_MONITOR_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <vector>

#include "base/common/sig.h"
#include "browser/services/play/play_service.h"
#include "browser/widgets/timeline/timeline_widget.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "browser/widgets/monitor/actions.h"
#include "base/layout/action_bar.h"

namespace nlive {

class ITimelineWidgetService;
class IThemeService;
class Sequence;

namespace monitor_widget {

class SequenceView;

class MonitorWidget : public QDockWidget, public Sig {

private:
  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<PlayService> play_service_;

  std::vector<sig2_conn_t> widget_connections_;
  SequenceView* sequence_view_;
  
  QSharedPointer<ActionContext> action_context_;
  ActionBar* action_bar_;

  void handleDidChangeCurrentTimelineWidget(timeline_widget::TimelineWidget* timeline_widget);
  void handleDidChangeSequenceView(timeline_widget::SequenceView* sequence_view);

protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

public:
  MonitorWidget(QWidget* parent,
    QSharedPointer<ITimelineWidgetService> timeline_widget_service,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<PlayService> play_service);

};

}

}

#endif