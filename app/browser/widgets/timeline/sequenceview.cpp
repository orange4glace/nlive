#include "./sequenceview.h"

namespace nlive {

namespace timelinewidget {

SequenceView::SequenceView(Sequence* sequence, IThemeService* theme_service) :
  side_view_(nullptr, sequence, theme_service),
  timeline_view_(nullptr, sequence, theme_service) {

}

SequenceSideView* SequenceView::side_view() {
  return &side_view_;
}
SequenceTimelineView* SequenceView::timeline_view() {
  return &timeline_view_;
}

}

}