#ifndef _NLIVE_TIMELINE_WIDGET_SEQUENCE_TIMELINE_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_SEQUENCE_TIMELINE_VIEW_H_

#include <QWidget>
#include <vector>

#include "browser/widgets/timeline/scrollview/sequencescrollview.h"

namespace nlive {

class Track;
class Sequence;
class IThemeService;

namespace timelinewidget {

class TrackTimelineView;

class SequenceTimelineView : public QWidget {
  Q_OBJECT

private:
  IThemeService* theme_service_;

  Sequence* sequence_;
  nlive::SequenceScrollView scroll_view_;

  std::vector<TrackTimelineView*> track_views_;
  
private slots:
  void handleDidAddTrack(Track* const track, int index);
  void handleWillRemoveTrack(Track* const track, int index);

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  SequenceTimelineView(
    QWidget* parent,
    Sequence* const sequence,
    IThemeService* const themeService);

  SequenceScrollView* scroll_view();

};

}

}

#endif