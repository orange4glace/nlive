#ifndef NLIVE_TIMELINE_WIDGET_SEQUENCE_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_SEQUENCE_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include "base/layout/flex_layout.h"
#include "./sequencesideview.h"
#include "./sequencetimelineview.h"
#include "base/layout/fillparentbox.h"
#include "browser/services/play/play_service.h"
#include "browser/widgets/timeline/sequence_playable.h"
#include "browser/widgets/timeline/sequencesideview.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "browser/widgets/timeline/sequencetimelineview.h"

namespace nlive {

namespace timeline_widget {

class SequenceView : public FlexLayout {

private:
  sptr<PlayService> play_service_;

  sptr<Sequence> sequence_;
  sptr<SequencePlayable> sequence_playable_;

  SequenceSideView* side_view_;
  
  SequenceScrollView* scroll_view_;
  SequenceTimelineView* timeline_view_;

public:
  SequenceView(QWidget* parent, sptr<Sequence> sequence,
    sptr<IThemeService> theme_service,
    sptr<PlayService> play_service);
  ~SequenceView();

  SequenceSideView* side_view();
  SequenceTimelineView* timeline_view();

  inline sptr<Sequence> sequence() { return sequence_; }
  inline sptr<SequencePlayable> sequence_playable() { return sequence_playable_; }


};

}

}

#endif