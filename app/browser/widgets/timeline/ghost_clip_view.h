#ifndef NLIVE_TIMELINE_WIDGET_GHOST_CLIP_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_GHOST_CLIP_VIEW_H_

#include <QWidget>

namespace nlive {

class SequenceScrollView;

namespace timeline_widget {

class GhostClipView : public QWidget {

friend class GhostSequenceView;
friend class GhostTrackView;

private:
  SequenceScrollView* scroll_view_;

  int start_time_;
  int end_time_;
  int extended_start_time_;
  int extended_end_time_;

  void setExtendedTime(int start_time, int end_time);

  void updateView();

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  GhostClipView(SequenceScrollView* scroll_view, int start_time, int end_time);

  int start_time() const;
  int end_time() const;
  int extended_start_time() const;
  int extended_end_time() const;

};

}

}

#endif