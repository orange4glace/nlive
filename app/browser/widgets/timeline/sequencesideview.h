#ifndef _NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_SEQUENCE_SIDE_VIEW_H_

#include <QWidget>
#include <QVBoxLayout>

namespace nlive {

class Sequence;
class Track;
class IThemeService;

namespace timelinewidget {

class TrackSideView;

class SequenceSideView : public QWidget {

private:
  IThemeService* theme_service_;

  Sequence* sequence_;

  std::vector<TrackSideView*> track_views_;

private:
  void addTrackView(Track* track, int index);
  void removeTrackView(Track* track, int index);

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  SequenceSideView(QWidget* parent, Sequence* sequence, IThemeService* theme_service);

};

}

}

#endif