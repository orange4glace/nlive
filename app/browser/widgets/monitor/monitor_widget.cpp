#include "browser/widgets/monitor/monitor_widget.h"

#include <QDebug>

#include "browser/widgets/timeline/timeline_widget_service.h"
#include "browser/widgets/timeline/timelinewidget.h"
#include "browser/widgets/monitor/sequence_view.h"

namespace nlive {

namespace monitor_widget {

MonitorWidget::MonitorWidget(QWidget* parent,
  QSharedPointer<ITimelineWidgetService> timeline_widget_service,
  IThemeService* theme_service) :
  QDockWidget(parent),
  timeline_widget_service_(timeline_widget_service),
  theme_service_(theme_service),
  sequence_view_(nullptr) {

  handleDidChangeCurrentTimelineWidget(timeline_widget_service_->current_widget());
  connect(timeline_widget_service_.get(), &ITimelineWidgetService::onDidChangeCurrentWidget, 
    this, &MonitorWidget::handleDidChangeCurrentTimelineWidget);

}

void MonitorWidget::handleDidChangeCurrentTimelineWidget(timelinewidget::TimelineWidget* timeline_widget) {
  for (auto& connection : widget_connections_)
    disconnect(connection);
  widget_connections_.clear();
  if (sequence_view_) delete sequence_view_;
  sequence_view_ = nullptr;
    qDebug() << "timeline widget " << timeline_widget << "\n";
  if (timeline_widget != nullptr) {
    qDebug() << " Seq = > " << timeline_widget->sequence()<< "\n";
    handleDidChangeSequence(timeline_widget->sequence());
    widget_connections_.push_back(
      connect(timeline_widget, &timelinewidget::TimelineWidget::onDidChangeSequence,
        this, &MonitorWidget::handleDidChangeSequence));
  }
}

void MonitorWidget::handleDidChangeSequence(QSharedPointer<Sequence> sequence) {
    qDebug() << "seq " << sequence << "\n";
  if (sequence_view_ != nullptr) {
    delete sequence_view_;
  }
  sequence_view_ = nullptr;
  if (sequence) {
    qDebug() << "seq view create\n";
    sequence_view_ = new SequenceView(this, sequence);
  }
}


}

}