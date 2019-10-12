#include "browser/widgets/timeline/timelinewidget.h"

#include <QStyle>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "model/sequence/sequence.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "browser/widgets/timeline/timeline_widget_service.h"

#include <iostream>

namespace nlive {

namespace timelinewidget {

TimelineWidget::TimelineWidget(
  QWidget* parent,
  IThemeService* theme_service,
  QSharedPointer<ITimelineWidgetService> timeline_widget_service) :
  QDockWidget(parent),
  theme_service_(theme_service),
  timeline_widget_service_(timeline_widget_service),
  sequence_(nullptr),
  sequence_view_(nullptr),
  split_left_view_(this, nullptr),
  split_right_view_(this, nullptr) {
  setTitleBarWidget(new QWidget());
  setFocusPolicy(Qt::ClickFocus);
}

void TimelineWidget::resizeEvent(QResizeEvent* event) {
  // TODO : implementn SplitView
  split_left_view_.move(0, 0);
  split_left_view_.resize(200, height());
  split_right_view_.move(200, 0);
  split_right_view_.resize(width() - 200, height());
  QWidget::resizeEvent(event);
}

void TimelineWidget::focusInEvent(QFocusEvent* event) {
  timeline_widget_service_->setCurrentWidget(this);
}

void TimelineWidget::setSequence(QSharedPointer<Sequence> sequence) {
  if (sequence_ != nullptr) {
    sequence_ = nullptr;
    split_left_view_.setContent(nullptr);
    split_right_view_.setContent(nullptr);
    delete sequence_view_;
  }
  sequence_ = sequence;
  if (sequence == nullptr) {
    onDidChangeSequence(sequence);
    return;
  }
  qDebug() << "emiti seq chan " << sequence << "\n";
  sequence_view_ = new SequenceView(sequence, theme_service_);
  split_left_view_.setContent(sequence_view_->side_view());
  split_right_view_.setContent(sequence_view_->timeline_view()->scroll_view());
  onDidChangeSequence(sequence);
}

QSharedPointer<Sequence> TimelineWidget::sequence() {
  return sequence_;
}

SequenceView* TimelineWidget::sequence_view() {
  return sequence_view_;
}

}

}