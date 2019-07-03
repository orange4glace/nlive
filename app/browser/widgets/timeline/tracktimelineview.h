#ifndef _NLIVE_TIMELINE_WIDGET_TRACK_TIMELINE_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_TRACK_TIMELINE_VIEW_H_

#include <QWidget>
#include <set>
#include "browser/widgets/timeline/clipview.h"

#include <iostream>

namespace nlive {

class Track;
class Clip;
class SequenceScrollView;
class IThemeService;

namespace timelinewidget {

class TrackTimelineView : public QWidget {
  Q_OBJECT

private:
  SequenceScrollView* scrollView_;
  IThemeService* theme_service_;

  Track* track_;

  std::map<int, TimelineWidgetClipView*> clipViews_;

private slots:
  void handleDidAddClip(Clip* const track);
  void handleWillRemoveClip(Clip* const track);

protected:
  void paintEvent(QPaintEvent* event) override;

public:
  TrackTimelineView(
    QWidget* const parent,
    Track* const track,
    SequenceScrollView* const scrollView,
    IThemeService* const themeService);

  TimelineWidgetClipView* const getClipView(const Clip* const clip);

signals:
  // These signals are kind of a relay which will be emitted when TimelineWidegtClipView emits `onDid...Clip()`
  void onDidFocusClip();
  void onDidFocusBlur();

};

}

}

#endif