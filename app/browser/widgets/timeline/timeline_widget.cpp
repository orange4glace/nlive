#include "browser/widgets/timeline/timeline_widget.h"

#include <QDebug>
#include <QPainter>
#include <QShortcut>

#include "model/sequence/sequence.h"
#include "model/storage/sequence_storage_item.h"
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
  sptr<PlayService> play_service,
  sptr<IProjectsService> projects_service) :
  Widget(parent, theme_service),
  theme_service_(theme_service), play_service_(play_service),
  timeline_widget_service_(timeline_widget_service), projects_service_(projects_service),
  sequence_storage_item_(nullptr),
  sequence_view_(nullptr) {
  setTitleBarWidget(new QWidget());

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

void TimelineWidget::onFocused() {
  timeline_widget_service_->setCurrentWidget(this);
  if (sequence_storage_item_) {
    projects_service_->setTargetProject(sequence_storage_item_->project());
  }
  else {
    projects_service_->setTargetProject(nullptr);
  }
}

void TimelineWidget::onBlured() {
  projects_service_->setTargetProject(nullptr);
}

void TimelineWidget::setSequenceStorageItem(sptr<SequenceStorageItem> sequence_storage_item) {
  if (sequence_storage_item_ != nullptr) {
    sequence_storage_item_ = nullptr;
    delete sequence_view_;
  }
  sequence_storage_item_ = sequence_storage_item;
  if (sequence_storage_item_ == nullptr) {
    onDidChangeSequenceStorageItem(nullptr);
    if (focused()) {
      projects_service_->setTargetProject(nullptr);
    }
    return;
  }
  auto sequence = sequence_storage_item_->sequence();
  sequence_view_ = new SequenceView(this, sequence, theme_service_, play_service_);
  sequence_view_->show();
  sequence_view_->setGeometry(rect());
  if (focused()) {
    projects_service_->setTargetProject(sequence_storage_item_->project());
  }
  onDidChangeSequenceStorageItem(sequence_storage_item_);
}

sptr<SequenceStorageItem> TimelineWidget::sequence_storage_item() {
  return sequence_storage_item_;
}

SequenceView* TimelineWidget::sequence_view() {
  return sequence_view_;
}

}

}