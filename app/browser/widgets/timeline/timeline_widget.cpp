#include "browser/widgets/timeline/timeline_widget.h"

#include <QDebug>
#include <QPainter>
#include <QShortcut>

#include "model/sequence/sequence.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "browser/widgets/timeline/timeline_widget_service.h"

#include <iostream>

namespace nlive {

namespace timeline_widget {

const std::string TimelineWidget::TYPE = "widget.Timeline";

TimelineWidget::TimelineWidget(
  QWidget* parent,
  sptr<IThemeService> theme_service,
  sptr<ITimelineWidgetService> timeline_widget_service,
  sptr<PlayService> play_service) :
  QDockWidget(parent),
  theme_service_(theme_service), play_service_(play_service),
  timeline_widget_service_(timeline_widget_service),
  sequence_(nullptr),
  sequence_view_(nullptr) {
  setTitleBarWidget(new QWidget());
  setFocusPolicy(Qt::ClickFocus);

  QShortcut* SC_space = new QShortcut(QKeySequence(Qt::Key_Space), this, 0, 0, Qt::WidgetShortcut);
  connect(SC_space, &QShortcut::activated, [this]() {
    if (sequence_view_) {
      play_service_->toggle(sequence_view_->sequence_playable());
    }
  });
}

void TimelineWidget::resizeEvent(QResizeEvent* event) {
  if (sequence_view_) sequence_view_->setGeometry(rect());
}

void TimelineWidget::focusInEvent(QFocusEvent* event) {
  timeline_widget_service_->setCurrentWidget(this);
}

void TimelineWidget::setSequence(sptr<Sequence> sequence) {
  if (sequence_ != nullptr) {
    sequence_ = nullptr;
    delete sequence_view_;
  }
  sequence_ = sequence;
  if (sequence == nullptr) {
    onDidChangeSequence(sequence);
    return;
  }
  sequence_view_ = new SequenceView(this, sequence, theme_service_, play_service_);
  sequence_view_->show();
  sequence_view_->setGeometry(rect());
  onDidChangeSequence(sequence);
}

sptr<Sequence> TimelineWidget::sequence() {
  return sequence_;
}

SequenceView* TimelineWidget::sequence_view() {
  return sequence_view_;
}

}

}