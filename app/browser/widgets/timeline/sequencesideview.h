#ifndef NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
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
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<Sequence> sequence_;

  std::vector<TrackSideView*> track_views_;

private:
  void addTrackView(QSharedPointer<Track> track, int index);
  void removeTrackView(QSharedPointer<Track> track, int index);

protected:
  void contentRectUpdated() override;
  void paintEvent(QPaintEvent* event) override;

public:
  SequenceSideView(QWidget* parent, QSharedPointer<Sequence> sequence, QSharedPointer<IThemeService> theme_service);

};

}

}

#endif