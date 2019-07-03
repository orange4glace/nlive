#include "browser/widgets/timeline/timelinewidget.h"

#include <QStyle>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "model/sequence/sequence.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "platform/theme/themeservice.h"

#include <iostream>

namespace nlive {

namespace timelinewidget {

TimelineWidget::TimelineWidget(
  QWidget* parent,
  IThemeService* theme_service) :
  QDockWidget(parent),
  theme_service_(theme_service),
  sequence_(nullptr),
  sequence_view_(nullptr),
  split_left_view_(this, nullptr),
  split_right_view_(this, nullptr) {

}

void TimelineWidget::resizeEvent(QResizeEvent* event) {
  // TODO : implementn SplitView
  split_left_view_.move(0, 0);
  split_left_view_.resize(200, height());
  split_right_view_.move(200, 0);
  split_right_view_.resize(width() - 200, height());
  QWidget::resizeEvent(event);
}

void TimelineWidget::setSequence(Sequence* sequence) {
  if (sequence_ != nullptr) {
    sequence_ = nullptr;
    split_left_view_.setContent(nullptr);
    split_right_view_.setContent(nullptr);
    delete sequence_view_;
  }
  sequence_ = sequence_;
  if (sequence == nullptr) return;
  sequence_view_ = new SequenceView(sequence, theme_service_);
  split_left_view_.setContent(sequence_view_->side_view());
  split_right_view_.setContent(sequence_view_->timeline_view()->scroll_view());
}

}

}