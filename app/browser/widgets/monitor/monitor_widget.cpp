#include "browser/widgets/monitor/monitor_widget.h"

#include <QDebug>
#include <QPainter>
#include "base/layout/fillparentbox.h";
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
  Widget(parent, theme_service),
  timeline_widget_service_(timeline_widget_service), play_service_(play_service),
  theme_service_(theme_service),
  sequence_view_(nullptr) {

  container_ = new FlexLayout(this, FlexLayout::Row);

  sequence_view_container_ = new FillParentBox(nullptr);
  action_bar_container_ = new FillParentBox(nullptr);
  action_bar_container_->setPadding(Div::ALL, 10);
  action_bar_container_->setFlexBasis(10 * 2 + 20 + 8 * 2);
  action_bar_container_->setFlexShrink(0)->setFlexGrow(0);

  action_context_ = sptr<ActionContext>(new ActionContext());
  action_bar_ = new ActionBar(nullptr, theme_service);
  action_bar_->setIconSize(QSize(20, 20));
  action_bar_->setIconPadding(5);
  action_bar_->setAlignment(Alignment::Center);
  action_bar_->addAction(std::make_shared<ForwardCurrnetTimeAction>(action_context_, false));
  action_bar_->addAction(std::make_shared<ToggleAction>(action_context_,
    play_service));
  action_bar_->addAction(std::make_shared<ForwardCurrnetTimeAction>(action_context_, true));
  action_bar_container_->setContent(action_bar_);

  container_->addChild(sequence_view_container_);
  container_->addChild(action_bar_container_);
  sequence_view_container_->show();
  action_bar_container_->show();

  handleDidChangeCurrentTimelineWidget(timeline_widget_service_->current_widget());
  timeline_widget_service_->onDidChangeCurrentWidget.connect(
    sig2_t<void (timeline_widget::TimelineWidget*)>::slot_type(
    &MonitorWidget::handleDidChangeCurrentTimelineWidget, this, _1).track(__sig_scope_));
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
    auto conn = timeline_widget->onDidChangeSequenceStorageItem.connect(sig2_t<void (sptr<SequenceStorageItem>)>::slot_type(
      [this, timeline_widget](sptr<SequenceStorageItem>) {
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
  sequence_view_ = nullptr;
  action_context_->setSequence(nullptr);
  action_context_->setSequencePlayable(nullptr);
  if (timeline_widget_sequence_view) {
    sequence_view_ = new SequenceView(nullptr, timeline_widget_sequence_view, play_service_);
    sequence_view_container_->setContent(sequence_view_);
    sequence_view_->show();
    sequence_view_->resize(size());
    action_context_->setSequence(timeline_widget_sequence_view->sequence());
    action_context_->setSequencePlayable(timeline_widget_sequence_view->sequence_playable());
  }
}

void MonitorWidget::resizeEvent(QResizeEvent* event) {
  container_->resize(size());
}


}

}