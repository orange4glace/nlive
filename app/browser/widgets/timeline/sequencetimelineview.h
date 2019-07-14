#ifndef _NLIVE_TIMELINE_WIDGET_SEQUENCE_TIMELINE_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_SEQUENCE_TIMELINE_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <set>
#include <vector>

#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "browser/widgets/timeline/ghost_sequence_view.h"

namespace nlive {

class Track;
class Sequence;
class IThemeService;

namespace timelinewidget {

class TrackTimelineView;
class ClipView;

namespace {

enum ManipulateState {
  IDLE,
  RESIZE_START,
  RESIZE_END,
  CLIP_BAR_PRESSED,
  CLIP_TRANSLATE,
  DRAG_AND_DROP,
};

}

class SequenceTimelineView : public QWidget {
  Q_OBJECT

private:
  IThemeService* theme_service_;

  QSharedPointer<Sequence> sequence_;
  nlive::SequenceScrollView scroll_view_;

  ManipulateState manipulate_state_;
  GhostSequenceView* ghost_sequence_view_;

  std::vector<TrackTimelineView*> track_views_;

  std::set<ClipView*> focused_clip_views_;

/*#region Manipulate*/

  QPoint manipulate_start_mouse_pos_;
  QPoint last_mouse_pos_;
  int last_time_pos_;
  int last_track_index_;
  std::vector<std::set<QSharedPointer<Clip>>> manipulate_target_clips_;

  void handleClipHandleMousePress(QMouseEvent* event);

  void handleClipBarPressedEvent(ClipView* clip_view, QMouseEvent* event);

  void startClipResizeLeft(ClipView* clip_view, QMouseEvent* event);
  void handleClipResizeLeft(QMouseEvent* event);
  void endClipResizeLeft(QMouseEvent* event);

  void startClipResizeRight(QMouseEvent* event);
  void handleClipResizeRight(QMouseEvent* event);
  void endClipResizeRight(QMouseEvent* event);

  void startClipTranslate(QMouseEvent* event);
  void handleClipTranslate(QMouseEvent* event);
  void endClipTranslate(QMouseEvent* event);

  void handleDrag(QDragMoveEvent* event);
  void handleDrop(QDropEvent* event);

  void updateGhostSequenceView();

/*#endregion*/
  
private slots:
  void handleDidAddTrack(QSharedPointer<Track> track, int index);
  void handleWillRemoveTrack(QSharedPointer<Track> track, int index);

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragLeaveEvent(QDragLeaveEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

public:
  SequenceTimelineView(
    QWidget* parent,
    QSharedPointer<Sequence> const sequence,
    IThemeService* const themeService);

  void blurAllClips();

  int getTrackIndexAtPoint(QPoint& point) const;
  int getTrackIndexAtPoint(QPoint&& point) const;
  SequenceScrollView* scroll_view();

};

}

}

#endif