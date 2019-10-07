#include "./sequenceview.h"

namespace nlive {

namespace timelinewidget {

SequenceView::SequenceView(QSharedPointer<Sequence> sequence, IThemeService* theme_service) :
  sequence_(sequence),
  side_view_(nullptr, sequence, theme_service),
  timeline_view_(nullptr, sequence, theme_service) {

}

SequenceSideView* SequenceView::side_view() {
  return &side_view_;
}
SequenceTimelineView* SequenceView::timeline_view() {
  return &timeline_view_;
}

QSharedPointer<Sequence> SequenceView::sequence() {
  return sequence_;
}

}

}