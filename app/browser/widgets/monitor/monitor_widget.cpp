#include "browser/widgets/monitor/monitor_widget.h"

#include <QDebug>
#include <QPainter>

#include "browser/widgets/timeline/timeline_widget_service.h"
#include "browser/widgets/timeline/timelinewidget.h"
#include "browser/widgets/monitor/sequence_view.h"

namespace nlive {

namespace monitor_widget {

MonitorWidget::MonitorWidget(QWidget* parent,
  QSharedPointer<ITimelineWidgetService> timeline_widget_service,
  QSharedPointer<IThemeService> theme_service) :
  QDockWidget(parent),
  timeline_widget_service_(timeline_widget_service),
  theme_service_(theme_service),
  sequence_view_(nullptr) {

  qDebug() << "MONITOR WIDGET CREATED\n";
  handleDidChangeCurrentTimelineWidget(timeline_widget_service_->current_widget());
  timeline_widget_service_->onDidChangeCurrentWidget.connect(
    sig2_t<void (timelinewidget::TimelineWidget*)>::slot_type(
    &MonitorWidget::handleDidChangeCurrentTimelineWidget, this, _1).track(__sig_scope_));
}

void MonitorWidget::handleDidChangeCurrentTimelineWidget(timelinewidget::TimelineWidget* timeline_widget) {
  for (auto& connection : widget_connections_)
    connection.disconnect();
  widget_connections_.clear();
  if (sequence_view_) {
    // TODO
    // delete sequence_view_;
  }
  sequence_view_ = nullptr;
  if (timeline_widget != nullptr) {
    handleDidChangeSequence(timeline_widget->sequence());
    auto conn = timeline_widget->onDidChangeSequence.connect(
      boost::bind(&MonitorWidget::handleDidChangeSequence, this, _1));
    widget_connections_.push_back(conn);
  }
}

void MonitorWidget::handleDidChangeSequence(QSharedPointer<Sequence> sequence) {
  if (sequence_view_ != nullptr) {
    // TODO
    // delete sequence_view_;
  }
  sequence_view_ = nullptr;
  if (sequence) {
    sequence_view_ = new SequenceView(this, sequence);
    sequence_view_->show();
    sequence_view_->resize(size());
  }
}

void MonitorWidget::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), Qt::darkYellow);
}

void MonitorWidget::resizeEvent(QResizeEvent* event) {
  if (sequence_view_) {
    sequence_view_->resize(size());
  }
}


}

}