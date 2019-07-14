#include "browser/widgets/timeline/ghost_sequence_view.h"

#include <limits>
#include <QPainter>

#include "model/sequence/sequence.h"
#include "browser/widgets/timeline/ghost_track_view.h"
#include "browser/widgets/timeline/ghost_clip_view.h"

namespace nlive {

namespace timelinewidget {

GhostSequenceView::GhostSequenceView(
  QWidget* parent,
  SequenceScrollView* scroll_view,
  QSharedPointer<Sequence> sequence) :
  QWidget(parent),
  sequence_(sequence), scroll_view_(scroll_view), on_schedule_(false) {
    
  setAttribute(Qt::WA_TransparentForMouseEvents);
  reset();

  // Prepare GhostTrackViews
  for (auto track : sequence->tracks()) {
    auto ghost_track_view = new GhostTrackView(this, scroll_view_);
    ghost_track_views_.push_back(ghost_track_view);
    connect(ghost_track_view, &GhostTrackView::onDidAddGhostClipView, this, [this]() {
      scheduleUpdate();
    });
    qDebug() << " " << ghost_track_view->isVisible() << "\n";
  }

  for (auto track : sequence->tracks()) {
    addTrackListener(track);
  }
  connect(sequence.get(), &Sequence::onDidAddTrack, this, [this](QSharedPointer<Track> track) {
    addTrackListener(track);
  });
  connect(sequence.get(), &Sequence::onWillRemoveTrack, this, [this](QSharedPointer<Track> track) {
    removeTrackListener(track);
  });

}

void GhostSequenceView::doUpdate() {
  for (auto gtv : ghost_track_views_) {
    for (auto ghost_clip : gtv->ghost_clip_views()) {
      int start_time = ghost_clip->start_time();
      int end_time = ghost_clip->end_time();
      int extended_start_time = start_time + start_extent_ + translation_ + start_magnet_time_;
      int extended_end_time = end_time + end_extent_ + translation_ + end_magnet_time_;

      ghost_clip->setExtendedTime(extended_start_time, extended_end_time);
    }
  }
  
  for (int i = 0; i < ghost_track_views_.size(); i ++) {
    int j = i - track_offset_;
    if (j < 0 || j >= ghost_track_views_.size()) continue;
    auto& track_view = ghost_track_views_[j];
    track_view->resize(width(), 29);
    track_view->move(0, i * 30);
  }
  
  QWidget::update();
}

void GhostSequenceView::scheduleUpdate() {
  if (on_schedule_) return;
  schedule_timer_.singleShot(30, this, [this]() {
    on_schedule_ = false;
    doUpdate();
  });
  on_schedule_ = true;
}

void GhostSequenceView::update() {
  scheduleUpdate();
}

GhostTrackView* GhostSequenceView::getGhostTrackView(int index) {
  return ghost_track_views_[index];
}

void GhostSequenceView::addTrackListener(QSharedPointer<Track> track) {
  for (auto clip : track->clips()) {
    addMagnetTime(clip->start_time());
    addMagnetTime(clip->end_time());
  }
  std::vector<QMetaObject::Connection> connections;
  connections.emplace_back(connect(track.get(), &Track::onDidAddClip, this, [this](QSharedPointer<Clip> clip) {
    addMagnetTime(clip->start_time());
    addMagnetTime(clip->end_time());
  }));
  connections.emplace_back(connect(track.get(), &Track::onWillRemoveClip, this, [this](QSharedPointer<Clip> clip) {
    deleteMagnetTime(clip->start_time());
    deleteMagnetTime(clip->end_time());
  }));
  connections.emplace_back(connect(track.get(), &Track::onDidChangeClipTime, this,
    [this](QSharedPointer<Clip> clip, int old_start_time, int old_end_time, int old_b_time) {
    deleteMagnetTime(old_start_time);
    deleteMagnetTime(old_end_time);
    addMagnetTime(clip->start_time());
    addMagnetTime(clip->end_time());
  }));
  track_connections_.insert({ track, connections });
}

void GhostSequenceView::removeTrackListener(QSharedPointer<Track> track) {
  for (auto clip : track->clips()) {
    deleteMagnetTime(clip->start_time());
    deleteMagnetTime(clip->end_time());
  }
  auto connections = track_connections_.find(track);
  for (auto& connection : connections->second) disconnect(connection);
  track_connections_.erase(connections);
}

void GhostSequenceView::addMagnetTime(int time) {
  int val = 1;
  auto bef = magnet_times_.find(time);
  if (bef != magnet_times_.end()) val = bef->second + 1;
  magnet_times_.insert({ time, val });
  scheduleUpdate();
}

void GhostSequenceView::deleteMagnetTime(int time) {
  int val = 0;
  auto bef = magnet_times_.find(time);
  if (bef == magnet_times_.end()) return;
  if (bef->second == 1) magnet_times_.erase(time);
  else magnet_times_.insert({ time, bef->second - 1 });
  scheduleUpdate();
}

void GhostSequenceView::reset() {
  magnet_start_ = false;
  magnet_end_ = false;
  start_extent_ = 0;
  max_start_extent_ = std::numeric_limits<int>::max();
  min_start_extent_ = std::numeric_limits<int>::min();
  
  end_extent_ = 0;
  max_end_extent_ = std::numeric_limits<int>::max();
  min_end_extent_ = std::numeric_limits<int>::min();
  
  translation_ = 0;
  max_translation_ = std::numeric_limits<int>::max();
  min_translation_ = std::numeric_limits<int>::min();

  track_offset_ = 0;
  max_track_offset_ = std::numeric_limits<int>::max();
  min_track_offset_ = std::numeric_limits<int>::min();

  magnet_threshold_ = 0;
  start_magnet_time_ = 0;
  end_magnet_time_ = 0;
  translation_magnet_time_ = 0;
}

void GhostSequenceView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), QColor(30,50,10,125));
}

void GhostSequenceView::resizeEvent(QResizeEvent* event) {
  doUpdate();
}

bool GhostSequenceView::magnet_start() const {
  return magnet_start_;
}

bool GhostSequenceView::magnet_end() const {
  return magnet_end_;
}

int GhostSequenceView::start_extent() const {
  return start_extent_;
}
int GhostSequenceView::max_start_extent() const {
  return max_start_extent_;
}
int GhostSequenceView::min_start_extent() const {
  return min_start_extent_;
}


int GhostSequenceView::end_extent() const {
  return end_extent_;
}
int GhostSequenceView::max_end_extent() const {
  return max_end_extent_;
}
int GhostSequenceView::min_end_extent() const {
  return min_end_extent_;
}

int GhostSequenceView::translation() const {
  return translation_;
}
int GhostSequenceView::max_translation() const {
  return max_translation_;
}
int GhostSequenceView::min_translation() const {
  return min_translation_;
}

int GhostSequenceView::track_offset() const {
  return track_offset_;
}
int GhostSequenceView::max_track_offset() const {
  return max_track_offset_;
}
int GhostSequenceView::min_track_offset() const {
  return min_track_offset_;
}


int GhostSequenceView::magnet_threshold() const {
  return magnet_threshold_;
}
int GhostSequenceView::start_magnet_time() const {
  return start_magnet_time_;
}
int GhostSequenceView::end_magnet_time() const {
  return end_magnet_time_;
}
int GhostSequenceView::translation_magnet_time() const {
  return translation_magnet_time_;
}

void GhostSequenceView::setMagnetStart(bool value) {
  magnet_start_ = value;
  scheduleUpdate();
}
void GhostSequenceView::setMagnetEnd(bool value) {
  magnet_end_ = value;
  scheduleUpdate();
}

void GhostSequenceView::setStartExtent(int value) {
  start_extent_ = qMax(min_start_extent_, qMin(max_start_extent_, value));
  scheduleUpdate();
}
void GhostSequenceView::setMaxStartExtent(int value) {
  max_start_extent_ = value;
  start_extent_ = qMin(start_extent_, value);
  scheduleUpdate();
}
void GhostSequenceView::setMinStartExtent(int value) {
  min_start_extent_ = value;
  start_extent_ = qMax(start_extent_, value);
  scheduleUpdate();
}


void GhostSequenceView::setEndExtent(int value) {
  end_extent_ = qMax(min_end_extent_, qMin(max_end_extent_, value));
  scheduleUpdate();
}
void GhostSequenceView::setMaxEndExtent(int value) {
  max_end_extent_ = value;
  end_extent_ = qMin(end_extent_, value);
  scheduleUpdate();
}
void GhostSequenceView::setMinEndExtent(int value) {
  min_end_extent_ = value;
  end_extent_ = qMax(end_extent_, value);
  scheduleUpdate();
}


void GhostSequenceView::setTranslation(int value) {
  translation_ = qMax(min_translation_, qMin(max_translation_, value));
  scheduleUpdate();
}
void GhostSequenceView::setMaxTranslation(int value) {
  max_translation_ = value;
  translation_ = qMin(translation_, value);
  scheduleUpdate();
}
void GhostSequenceView::setMinTranslation(int value) {
  min_translation_ = value;
  translation_ = qMax(translation_, value);
  scheduleUpdate();
}

void GhostSequenceView::setTrackOffset(int value) {
  track_offset_ = qMax(min_track_offset_, qMin(max_track_offset_, value));
  scheduleUpdate();
}
void GhostSequenceView::setMaxTrackOffset(int value) {
  max_track_offset_ = value;
  track_offset_ = qMin(track_offset_, value);
  scheduleUpdate();
}
void GhostSequenceView::setMinTrackOffset(int value) {
  min_track_offset_ = value;
  track_offset_ = qMax(track_offset_, value);
  scheduleUpdate();
}

void GhostSequenceView::setMagnetThreshold(int value) {
  magnet_threshold_ = value;
  scheduleUpdate();
}

}

}