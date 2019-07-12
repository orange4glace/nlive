#ifndef _NLIVE_GHOST_SEQUENCE_VIEW_H_
#define _NLIVE_GHOST_SEQUENCE_VIEW_H_

#include <map>
#include <QWidget>
#include <QSharedPointer>
#include <QMetaObject>
#include <QTimer>
#include "model/sequence/track.h"

namespace nlive {

class Sequence;
class SequenceScrollView;

namespace timelinewidget {

class GhostTrackView;
class GhostClipView;

class GhostSequenceView : public QWidget {
  Q_OBJECT

private:
  QSharedPointer<Sequence> sequence_;
  std::vector<GhostTrackView*> ghost_track_views_;

  SequenceScrollView* scroll_view_;

  std::map<int, int> magnet_times_;
  std::map<QSharedPointer<Track>, std::vector<QMetaObject::Connection>, TrackCompare> track_connections_;

  bool magnet_start_;
  bool magnet_end_;

  int start_extent_;
  int max_start_extent_;
  int min_start_extent_;

  int end_extent_;
  int max_end_extent_;
  int min_end_extent_;

  int translation_;
  int max_translation_;
  int min_translation_;

  int track_offset_;
  int max_track_offset_;
  int min_track_offset_;

  int magnet_threshold_;
  int start_magnet_time_;
  int end_magnet_time_;
  int translation_magnet_time_;

  QTimer schedule_timer_;
  bool on_schedule_;

  void scheduleUpdate();
  void doUpdate();

  void addGhostTrackView();

  void addTrackListener(QSharedPointer<Track> track);
  void removeTrackListener(QSharedPointer<Track> track);

protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

public:
  GhostSequenceView(QWidget* parent, SequenceScrollView* scroll_view, QSharedPointer<Sequence> sequence);

  int getClosestDeltaTime(int time) const;
  void addMagnetTime(int time);
  void deleteMagnetTime(int time);
  void update();
  void reset();

  GhostTrackView* getGhostTrackView(int index);

  bool magnet_start() const;
  bool magnet_end() const;

  int start_extent() const;
  int max_start_extent() const;
  int min_start_extent() const;

  int end_extent() const;
  int max_end_extent() const;
  int min_end_extent() const;

  int translation() const;
  int max_translation() const;
  int min_translation() const;

  int track_offset() const;
  int max_track_offset() const;
  int min_track_offset() const;

  int magnet_threshold() const;
  int start_magnet_time() const;
  int end_magnet_time() const;
  int translation_magnet_time() const;

  void setMagnetStart(bool value);
  void setMagnetEnd(bool value);

  void setStartExtent(int value);
  void setMaxStartExtent(int value);
  void setMinStartExtent(int value);

  void setEndExtent(int value);
  void setMaxEndExtent(int value);
  void setMinEndExtent(int value);

  void setTranslation(int value);
  void setMaxTranslation(int value);
  void setMinTranslation(int value);

  void setTrackOffset(int value);
  void setMaxTrackOffset(int value);
  void setMinTrackOffset(int value);

  void setMagnetThreshold(int value);

};

}

}

#endif