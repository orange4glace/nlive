#ifndef _NLIVE_TIMELINE_WIDGET_TRACK_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_TRACK_VIEW_H_

#include "browser/widgets/timeline/tracksideview.h"
#include "browser/widgets/timeline/tracktimelineview.h"

#include <QWidget>
#include <QSharedPointer>
#include <QLayout>

namespace nlive {

class Track;
class IThemeService;
class SequenceScrollView;

namespace timelinewidget {

class TrackView {

private:
  TrackSideView side_view_;
  TrackTimelineView timeline_view_;

public:
  TrackView(QSharedPointer<Track> track, SequenceScrollView* scroll_view, IThemeService* theme_service);

  TrackSideView* side_view();
  TrackTimelineView* timeline_view();
  

};

}

}

#endif