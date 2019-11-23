#ifndef NLIVE_TIMELINE_WIDGET_TRACK_TIMELINE_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_TRACK_TIMELINE_VIEW_H_

#include <QWidget>
#include "base/common/memory.h"
#include <set>
#include <map>
#include <iostream>

#include "base/common/sig.h"
#include "model/sequence/clip.h"
#include "browser/widgets/timeline/clipview.h"


namespace nlive {

class Track;
class SequenceScrollView;
class IThemeService;

namespace timeline_widget {

class TrackTimelineView : public QWidget, public Sig {
  Q_OBJECT

private:
  SequenceScrollView* scrollView_;
  sptr<IThemeService> theme_service_;

  sptr<Track> track_;

  std::set<ClipView*> clip_views_;
  std::set<ClipView*> focused_clip_views_;
  std::map<sptr<Clip>, ClipView*, ClipStartCompare> clip_to_view_map_;

  void doLayout();

private slots:
  void handleDidAddClip(sptr<Clip> track);
  void handleWillRemoveClip(sptr<Clip> track);


protected:
  bool event(QEvent* event) override;

public:
  TrackTimelineView(
    QWidget* const parent,
    sptr<Track> const track,
    SequenceScrollView* const scrollView,
    sptr<IThemeService> const themeService);

  void updateClipViews();

  ClipView* const getClipView(sptr<Clip> clip);
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