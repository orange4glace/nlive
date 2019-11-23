#include "./sequenceview.h"

#include <QDebug>
#include <QShortcut>

namespace nlive {

namespace timeline_widget {

// TODO : This sometimes crashes.
// This may be because of the montior_widget::SequenceView
SequenceView::SequenceView(QWidget* parent, sptr<Sequence> sequence,
  sptr<IThemeService> theme_service,
  sptr<PlayService> play_service) :
  FlexLayout(parent), sequence_(sequence), play_service_(play_service) {
  side_view_ = new SequenceSideView(this, sequence, theme_service);
  scroll_view_ = new SequenceScrollView(this, nullptr, sequence, theme_service);
  timeline_view_ = new SequenceTimelineView(scroll_view_, sequence, theme_service);
  side_view_->setFlexBasis(200)->setFlexShrink(0)->setFlexGrow(0);
  addChild(side_view_);
  addChild(scroll_view_);
  sequence_playable_ = sptr<SequencePlayable>(
      new SequencePlayable(this, sequence));
}

SequenceView::~SequenceView() {
  play_service_->stop(sequence_playable_);
}

SequenceSideView* SequenceView::side_view() {
  return side_view_;
}

SequenceTimelineView* SequenceView::timeline_view() {
  return timeline_view_;
}

}

}