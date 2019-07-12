#include "browser/widgets/timeline/trackview.h"

#include "platform/theme/themeservice.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"

namespace nlive {

namespace timelinewidget {

TrackView::TrackView(QSharedPointer<Track> track, SequenceScrollView* scroll_view, IThemeService* theme_service) :
  side_view_(nullptr, track, theme_service),
  timeline_view_(nullptr, track, scroll_view, theme_service) {

}

TrackSideView* TrackView::side_view()  {
  return &side_view_;
}

TrackTimelineView* TrackView::timeline_view() {
  return &timeline_view_;
}

}

}