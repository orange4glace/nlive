#ifndef _NLIVE_TIMELINE_WIDGET_TRACK_TIMELINE_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_TRACK_TIMELINE_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <set>
#include <map>
#include <iostream>

#include "model/sequence/clip.h"
#include "browser/widgets/timeline/clipview.h"


namespace nlive {

class Track;
class SequenceScrollView;
class IThemeService;

namespace timelinewidget {

class TrackTimelineView : public QWidget {
  Q_OBJECT

private:
  SequenceScrollView* scrollView_;
  IThemeService* theme_service_;

  QSharedPointer<Track> track_;

  std::set<ClipView*> clip_views_;
  std::set<ClipView*> focused_clip_views_;
  std::map<QSharedPointer<Clip>, ClipView*, ClipCompare> clip_to_view_map_;

private slots:
  void handleDidAddClip(QSharedPointer<Clip> track);
  void handleWillRemoveClip(QSharedPointer<Clip> track);


protected:
  void mouseMoveEvent(QMouseEvent* event) override;

  void paintEvent(QPaintEvent* event) override;

public:
  TrackTimelineView(
    QWidget* const parent,
    QSharedPointer<Track> const track,
    SequenceScrollView* const scrollView,
    IThemeService* const themeService);

  ClipView* const getClipView(const Clip* const clip);
  void blurAllClips();

  const std::set<ClipView*>& clip_views();
  const std::set<ClipView*>& focused_clip_views();

signals:
  // These signals are kind of a relay which will be emitted when TimelineWidegtClipView emits `onDid...Clip()`
  void onDidFocusClip(ClipView* clip_view);
  void onDidBlurClip(ClipView* clip_view);

};

}

}

#endif