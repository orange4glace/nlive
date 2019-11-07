#ifndef NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <QVBoxLayout>
#include "base/common/sig.h"

namespace nlive {

class Sequence;
class Track;
class IThemeService;

namespace timelinewidget {

class TrackSideView;

class SequenceSideView : public QWidget, public Sig {

private:
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<Sequence> sequence_;

  std::vector<TrackSideView*> track_views_;

private:
  void addTrackView(QSharedPointer<Track> track, int index);
  void removeTrackView(QSharedPointer<Track> track, int index);

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  SequenceSideView(QWidget* parent, QSharedPointer<Sequence> sequence, QSharedPointer<IThemeService> theme_service);

};

}

}

#endif