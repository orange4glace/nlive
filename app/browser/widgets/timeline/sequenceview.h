#ifndef _NLIVE_TIMELINE_WIDGET_SEQUENCE_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_SEQUENCE_VIEW_H_

#include <QSharedPointer>

#include "./sequencesideview.h"
#include "./sequencetimelineview.h"

namespace nlive {

namespace timelinewidget {

class SequenceSideView;
class SequenceTimelineView;

class SequenceView {

private:
  QSharedPointer<Sequence> sequence_;
  SequenceSideView side_view_;
  SequenceTimelineView timeline_view_;

public:
  SequenceView(QSharedPointer<Sequence> sequence, IThemeService* theme_service);

  SequenceSideView* side_view();
  SequenceTimelineView* timeline_view();

  QSharedPointer<Sequence> sequence();


};

}

}

#endif