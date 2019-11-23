#include "browser/widgets/monitor/monitor_widget.h"

#include <QDebug>
#include <QPainter>

#include "browser/widgets/timeline/timeline_widget_service.h"
#include "browser/widgets/timeline/timeline_widget.h"
#include "browser/widgets/monitor/sequence_view.h"

namespace nlive {

namespace monitor_widget {

const std::string MonitorWidget::TYPE = "widget.Monitor";

MonitorWidget::MonitorWidget(QWidget* parent,
  sptr<ITimelineWidgetService> timeline_widget_service,
  sptr<IThemeService> theme_service,
  sptr<PlayService> play_service) :
  QDockWidget(parent),
  timeline_widget_service_(timeline_widget_service), play_service_(play_service),
  theme_service_(theme_service),
  sequence_view_(nullptr) {

  action_context_ = sptr<ActionContext>(new ActionContext());
  action_bar_ = new ActionBar(this, theme_service);
  action_bar_->setIconSize(QSize(20, 20));

  handleDidChangeCurrentTimelineWidget(timeline_widget_service_->current_widget());
  timeline_widget_service_->onDidChangeCurrentWidget.connect(
    sig2_t<void (timeline_widget::TimelineWidget*)>::slot_type(
    &MonitorWidget::handleDidChangeCurrentTimelineWidget, this, _1).track(__sig_scope_));

  action_bar_->addAction(new PlayPauseAction(action_bar_, action_context_, play_service), ":/browser/play-button.svg");
}

void MonitorWidget::handleDidChangeCurrentTimelineWidget(timeline_widget::TimelineWidget* timeline_widget) {
  for (auto& connection : widget_connections_)
    connection.disconnect();
  widget_connections_.clear();
  if (sequence_view_) {
    // TODO
    // delete sequence_view_;
  }
  sequence_view_ = nullptr;
  action_context_->setSequence(nullptr);
  action_context_->setSequencePlayable(nullptr);
  if (timeline_widget != nullptr) {
    handleDidChangeSequenceView(timeline_widget->sequence_view());
    auto conn = timeline_widget->onDidChangeSequence.connect(sig2_t<void (sptr<Sequence> sequence)>::slot_type([this, timeline_widget](sptr<Sequence> sequence) {
      handleDidChangeSequenceView(timeline_widget->sequence_view());
    }));
    widget_connections_.push_back(conn);
  }
}

void MonitorWidget::handleDidChangeSequenceView(timeline_widget::SequenceView* timeline_widget_sequence_view) {
  if (sequence_view_ != nullptr) {
    // TODO
    delete sequence_view_;
  }
  qDebug() << "DONE DELETE";
  sequence_view_ = nullptr;
  action_context_->setSequence(nullptr);
  action_context_->setSequencePlayable(nullptr);
  if (timeline_widget_sequence_view) {
    sequence_view_ = new SequenceView(this, timeline_widget_sequence_view, play_service_);
    sequence_view_->show();
    sequence_view_->resize(size());
    action_context_->setSequence(timeline_widget_sequence_view->sequence());
    action_context_->setSequencePlayable(timeline_widget_sequence_view->sequence_playable());
  }
}

void MonitorWidget::paintEvent(QPaintEvent* event) {
  QPainter p(this);
}

void MonitorWidget::resizeEvent(QResizeEvent* event) {
  int ACTION_BAR_HEIGHT = 20;
  if (sequence_view_) {
    sequence_view_->setGeometry(0, 0, width(), height() - ACTION_BAR_HEIGHT);
  }
  action_bar_->setGeometry(0, height() - ACTION_BAR_HEIGHT, width(), ACTION_BAR_HEIGHT);
}


}

}