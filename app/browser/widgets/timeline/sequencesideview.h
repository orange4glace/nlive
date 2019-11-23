#ifndef NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include <QVBoxLayout>
#include "base/layout/div.h"
#include "base/common/sig.h"

namespace nlive {

class Sequence;
class Track;
class IThemeService;

namespace timeline_widget {

class TrackSideView;

class SequenceSideView : public Div {

private:
  sptr<IThemeService> theme_service_;

  sptr<Sequence> sequence_;

  std::vector<TrackSideView*> track_views_;

private:
  void addTrackView(sptr<Track> track, int index);
  void removeTrackView(sptr<Track> track, int index);

protected:
  void contentRectUpdated() override;
  void paintEvent(QPaintEvent* event) override;

public:
  SequenceSideView(QWidget* parent, sptr<Sequence> sequence, sptr<IThemeService> theme_service);

};

}

}

#endif