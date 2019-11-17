#ifndef NLIVE_GHOST_TRACK_VIEW_H_
#define NLIVE_GHOST_TRACK_VIEW_H_

#include <QWidget>
#include <set>

namespace nlive {

class SequenceScrollView;

namespace timeline_widget {

class GhostClipView;

class GhostTrackView : public QWidget {
  Q_OBJECT

private:
  SequenceScrollView* scroll_view_;

  std::set<GhostClipView*> ghost_clip_views_;

protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

public:
  GhostTrackView(QWidget* parent, SequenceScrollView* scroll_view);
  void addGhostClipView(GhostClipView* view);
  void removeGhostClipView(GhostClipView* view);

  const std::set<GhostClipView*>& ghost_clip_views();

signals:
  void onDidAddGhostClipView(GhostClipView* view);
  void onWillRemoveGhostClipView(GhostClipView* view);

};

}

}

#endif