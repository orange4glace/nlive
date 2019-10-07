#include "browser/widgets/effect_control/sequence_view.h"

#include <QDebug>
#include <QPainter>

#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "browser/widgets/timeline/clipview.h"
#include "browser/widgets/effect_control/clip_view.h"

namespace nlive {

namespace effect_control {

SequenceView::SequenceView(
  QWidget* parent,
  QSharedPointer<EffectControlLayout> layout,
  timelinewidget::SequenceView* timeline_widget_sequence_view,
  QSharedPointer<IThemeService> theme_service) :
  QWidget(parent),
  theme_service_(theme_service),
  layout_(layout),
  timeline_widget_sequence_view_(timeline_widget_sequence_view),
  clip_(nullptr),
  clip_view_(nullptr) {
  this->handleDidChangeFocusedClips();
  connect(timeline_widget_sequence_view_->timeline_view(), &timelinewidget::SequenceTimelineView::onDidChangeFocusedClips, this, [this]() {
    this->handleDidChangeFocusedClips();
  });

}

void SequenceView::handleDidChangeFocusedClips() {
  auto& focused_clip_views = timeline_widget_sequence_view_->timeline_view()->focused_clip_views();
  if (focused_clip_views.size() != 1) return;
  auto clip = (*focused_clip_views.begin())->clip();
  createClipView(clip);
}

void SequenceView::createClipView(QSharedPointer<Clip> clip) {
  clearClipView();
  clip_ = clip;
  auto sequence = timeline_widget_sequence_view_->sequence();
  clip_view_ = new ClipView(this, layout_, sequence, clip, theme_service_);
  clip_view_->show();
}

void SequenceView::clearClipView() {
  if (clip_view_ == nullptr) return;
  delete clip_view_;
}

void SequenceView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), Qt::darkGreen);
}

void SequenceView::resizeEvent(QResizeEvent* event) {
  if (clip_view_ != nullptr) {
    clip_view_->resize(width(), clip_view_->sizeHint().height());
  }
}

}

}