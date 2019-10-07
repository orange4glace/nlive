#include "browser/widgets/timeline/sequencetimelineview.h"

#include "platform/theme/themeservice.h"
#include "model/sequence/track.h"
#include "model/sequence/sequence.h"
#include "model/storage/storage_item.h"
#include "model/storage/video_resource_storage_item.h"
#include "browser/drag/static_drag_and_drop.h"
#include "browser/widgets/timeline/trackview.h"
#include "browser/widgets/timeline/clipview.h"
#include "browser/widgets/timeline/ghost_clip_view.h"
#include "browser/widgets/timeline/ghost_track_view.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"
#include "browser/widgets/project/storage_item_view.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>

namespace nlive {

namespace timelinewidget {

SequenceTimelineView::SequenceTimelineView(
  QWidget* parent,
  QSharedPointer<Sequence> sequence,
  IThemeService* const theme_service) :
  QWidget(),
  sequence_(sequence),
  scroll_view_(parent, this, sequence, theme_service),
  manipulate_state_(ManipulateState::IDLE),
  ghost_sequence_view_(nullptr),
  theme_service_(theme_service) {
  setAcceptDrops(true);
  setParent(&scroll_view_);

  auto tracks = sequence->tracks();
  for (int i = 0; i < tracks.size(); i ++) handleDidAddTrack(tracks[i], i);
  QObject::connect(sequence.get(), &Sequence::onDidAddTrack, this, &SequenceTimelineView::handleDidAddTrack);
}

void SequenceTimelineView::handleDidAddTrack(QSharedPointer<Track> track, int index) {
  // Create and add Track view
  auto view = new TrackTimelineView(this, track, &scroll_view_, theme_service_);
  track_views_.emplace(track_views_.begin() + index, view);

  for (auto clip_view : view->focused_clip_views())
    focused_clip_views_.insert(clip_view);
  emit onDidChangeFocusedClips();
  connect(view, &TrackTimelineView::onDidFocusClip, this, [this](ClipView* clip_view) {
    focused_clip_views_.insert(clip_view);
    emit onDidChangeFocusedClips();
  });
  connect(view, &TrackTimelineView::onDidBlurClip, this, [this](ClipView* clip_view) {
    focused_clip_views_.erase(clip_view);
    emit onDidChangeFocusedClips();
  });
}

void SequenceTimelineView::handleWillRemoveTrack(QSharedPointer<Track> track, int index) {
  // Delete Track view
  auto track_view = track_views_[index];
  track_view->blurAllClips();
  track_views_.erase(track_views_.begin() + index);
  delete track_view;
}

void SequenceTimelineView::mousePressEvent(QMouseEvent* event) {
  handleClipHandleMousePress(event);
  last_mouse_pos_ = event->pos();
}

void SequenceTimelineView::mouseMoveEvent(QMouseEvent* event) {
  if (manipulate_state_ == ManipulateState::RESIZE_START) handleClipResizeLeft(event);
  if (manipulate_state_ == ManipulateState::CLIP_BAR_PRESSED) startClipTranslate(event);
  if (manipulate_state_ == ManipulateState::CLIP_TRANSLATE) handleClipTranslate(event);
  last_mouse_pos_ = event->pos();
}

void SequenceTimelineView::mouseReleaseEvent(QMouseEvent* event) {
  if (manipulate_state_ == ManipulateState::RESIZE_START) endClipResizeLeft(event);
  if (manipulate_state_ == ManipulateState::CLIP_BAR_PRESSED) manipulate_state_ = ManipulateState::IDLE;
  if (manipulate_state_ == ManipulateState::CLIP_TRANSLATE) endClipTranslate(event);
  last_mouse_pos_ = event->pos();
}

void SequenceTimelineView::dragEnterEvent(QDragEnterEvent* event) {
  auto mime_data = event->mimeData();
  if (mime_data->text() == "StorageItemView") {
    if (manipulate_state_ != ManipulateState::IDLE) return;
    event->accept();
    manipulate_state_ = ManipulateState::DRAG_AND_DROP;
    project_widget::StorageItemView* storage_item_view = 
      static_cast<project_widget::StorageItemView*>(StaticDragAndDrop::data());
    auto storage_item = storage_item_view->storage_item();
    auto clip = storage_item->cliperize(sequence_->time_base());

    ghost_sequence_view_ = new GhostSequenceView(this, &scroll_view_, sequence_);
    manipulate_target_clips_.clear();
    manipulate_target_clips_.resize(track_views_.size());
    Q_ASSERT(track_views_.size() > 0);
    auto& manipulate_track = manipulate_target_clips_[0];
    manipulate_track.insert(clip);
    ghost_sequence_view_->getGhostTrackView(0)->addGhostClipView(
      new GhostClipView(&scroll_view_, clip->start_time(), clip->end_time()));
    updateGhostSequenceView();
  }
  last_mouse_pos_ = event->pos();
}

void SequenceTimelineView::dragLeaveEvent(QDragLeaveEvent* event) {
  if (manipulate_state_ == ManipulateState::DRAG_AND_DROP) {
    manipulate_state_ = ManipulateState::IDLE;
    delete ghost_sequence_view_;
    manipulate_target_clips_.clear();
  }
}

void SequenceTimelineView::dragMoveEvent(QDragMoveEvent* event) {
  handleDrag(event);
  last_mouse_pos_ = event->pos();
}

void SequenceTimelineView::dropEvent(QDropEvent* event) {
  handleDrop(event);
  last_mouse_pos_ = event->pos();
}

void SequenceTimelineView::handleClipHandleMousePress(QMouseEvent* event) {
  int x = event->globalX();
  int y = event->globalY();
  auto point = QPoint(event->globalX(), event->globalY());

  ClipViewHandle handle = ClipViewHandle::NONE;
  ClipView* target_clip = nullptr;
  for (auto track_view : track_views_) {
    if (handle != ClipViewHandle::NONE) break;
    for (auto clip_view : track_view->clip_views()) {
      ClipViewHandle handle_ = clip_view->testHandle(clip_view->mapFromGlobal(point));
      handle = handle_;
      target_clip = clip_view;
      if (handle != ClipViewHandle::NONE) break;
    }
  }
  if (handle == ClipViewHandle::NONE) return;
  switch (handle) {
    case ClipViewHandle::BAR:
    handleClipBarPressedEvent(target_clip, event);
    break;
    case ClipViewHandle::LEFT_OUTER:
    case ClipViewHandle::LEFT_INNER:
    startClipResizeLeft(target_clip, event);
    break;
  }
}

void SequenceTimelineView::handleClipBarPressedEvent(ClipView* clip_view, QMouseEvent* event) {
  manipulate_state_ = ManipulateState::CLIP_BAR_PRESSED;
  int shift_pressed = event->modifiers() & Qt::ShiftModifier;
  if (!shift_pressed) {
    if (!clip_view->focused()) {
      blurAllClips();
      clip_view->focus();
    }
  }
  else {
    clip_view->focus();
  }
}



void SequenceTimelineView::startClipResizeLeft(ClipView* clip_view, QMouseEvent* event) {
  if (manipulate_state_ != ManipulateState::IDLE) return;
  manipulate_state_ = ManipulateState::RESIZE_START;
  // TODO : delete
  ghost_sequence_view_ = new GhostSequenceView(this, &scroll_view_, sequence_);
  manipulate_target_clips_.clear();
  manipulate_target_clips_.resize(track_views_.size());
  manipulate_start_mouse_pos_ = event->pos();
  for (int i = 0; i < track_views_.size(); i++) {
    auto track_view = track_views_[i];
    auto& manipulate_track = manipulate_target_clips_[i];
    auto& focused_clip_views = track_view->focused_clip_views();
    auto ghost_track_view = ghost_sequence_view_->getGhostTrackView(i);
    for (auto clip_view : focused_clip_views) {
      ghost_track_view->addGhostClipView(new GhostClipView(&scroll_view_, clip_view->clip()->start_time(), clip_view->clip()->end_time()));
      manipulate_track.insert(clip_view->clip());
    }
  }
  updateGhostSequenceView();
}

void SequenceTimelineView::handleClipResizeLeft(QMouseEvent* event) {
  if (manipulate_state_ != ManipulateState::RESIZE_START) return;
  QPoint d_mouse_pos = event->pos() - manipulate_start_mouse_pos_;
  int dt = scroll_view_.getTimeAmountRelativeToView(d_mouse_pos.x());
  ghost_sequence_view_->setStartExtent(dt);
}

void SequenceTimelineView::endClipResizeLeft(QMouseEvent* event) {
  // Detach clips
  for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
    auto& clips = manipulate_target_clips_[i];
    auto track = sequence_->tracks()[i];
    for (auto clip : clips) {
      track->detachClip(clip);
    }
  }
  int dt = ghost_sequence_view_->start_extent() + ghost_sequence_view_->magnet_start();
  for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
    auto& clips = manipulate_target_clips_[i];
    auto track = sequence_->tracks()[i];
    for (auto clip : clips) {
      clip->setTime(clip->start_time() + dt, clip->end_time(), clip->b_time() + dt);
    }
  }
  // Reattach clips
  for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
    auto& clips = manipulate_target_clips_[i];
    auto track = sequence_->tracks()[i];
    for (auto clip : clips) {
      track->attachClip(clip);
    }
  }

  delete ghost_sequence_view_;
  ghost_sequence_view_ = nullptr;
  manipulate_state_ = ManipulateState::IDLE;
}




void SequenceTimelineView::startClipTranslate(QMouseEvent* event) {
  if (manipulate_state_ != ManipulateState::CLIP_BAR_PRESSED) return;
  manipulate_state_ = ManipulateState::CLIP_TRANSLATE;
  ghost_sequence_view_ = new GhostSequenceView(this, &scroll_view_, sequence_);
  manipulate_target_clips_.clear();
  manipulate_target_clips_.resize(track_views_.size());
  manipulate_start_mouse_pos_ = event->pos();
  for (int i = 0; i < track_views_.size(); i++) {
    auto track_view = track_views_[i];
    auto& manipulate_track = manipulate_target_clips_[i];
    auto& focused_clip_views = track_view->focused_clip_views();
    auto ghost_track_view = ghost_sequence_view_->getGhostTrackView(i);
    for (auto clip_view : focused_clip_views) {
      ghost_track_view->addGhostClipView(new GhostClipView(&scroll_view_, clip_view->clip()->start_time(), clip_view->clip()->end_time()));
      manipulate_track.insert(clip_view->clip());
    }
  }
  updateGhostSequenceView();
}

void SequenceTimelineView::handleClipTranslate(QMouseEvent* event) {
  if (manipulate_state_ != ManipulateState::CLIP_TRANSLATE) return;
  QPoint d_mouse_pos = event->pos() - manipulate_start_mouse_pos_;
  int dt = scroll_view_.getTimeAmountRelativeToView(d_mouse_pos.x());
  int last_track_index = getTrackIndexAtPoint(last_mouse_pos_);
  int current_track_index = getTrackIndexAtPoint(event->pos());
  if (last_track_index != current_track_index) {
    int d_track_offset = current_track_index - last_track_index;
    ghost_sequence_view_->setTrackOffset(ghost_sequence_view_->track_offset() + d_track_offset);
  }
  ghost_sequence_view_->setTranslation(dt);
}

void SequenceTimelineView::endClipTranslate(QMouseEvent* event) {
  if (ghost_sequence_view_->track_offset() == 0) {
    // Detach clips
    for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
      auto& clips = manipulate_target_clips_[i];
      auto track = sequence_->tracks()[i];
      for (auto clip : clips) {
        track->detachClip(clip);
      }
    }
    int start_d_time = ghost_sequence_view_->start_extent() + ghost_sequence_view_->magnet_start() + ghost_sequence_view_->translation() + ghost_sequence_view_->translation_magnet_time();
    int end_d_time = ghost_sequence_view_->end_extent() + ghost_sequence_view_->magnet_end() + ghost_sequence_view_->translation() + ghost_sequence_view_->translation_magnet_time();
    int b_d_time = ghost_sequence_view_->start_extent() + ghost_sequence_view_->magnet_start();
    for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
      auto& clips = manipulate_target_clips_[i];
      auto track = sequence_->tracks()[i];
      for (auto clip : clips) {
        clip->setTime(clip->start_time() + start_d_time, clip->end_time() + end_d_time, clip->b_time() + b_d_time);
      }
    }
    // Reattach clips
    for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
      auto& clips = manipulate_target_clips_[i];
      auto track = sequence_->tracks()[i];
      for (auto clip : clips) {
        track->attachClip(clip);
      }
    }
  }
  else {
    // Remove clips
    for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
      auto& clips = manipulate_target_clips_[i];
      auto origin_track = sequence_->getTrackAt(i);
      Q_ASSERT(origin_track);
      for (auto clip : clips) {
        origin_track->removeClip(clip);
      }
    }
    // Add clips
    int start_d_time = ghost_sequence_view_->start_extent() + ghost_sequence_view_->magnet_start() + ghost_sequence_view_->translation() + ghost_sequence_view_->translation_magnet_time();
    int end_d_time = ghost_sequence_view_->end_extent() + ghost_sequence_view_->magnet_end() + ghost_sequence_view_->translation() + ghost_sequence_view_->translation_magnet_time();
    int b_d_time = ghost_sequence_view_->start_extent() + ghost_sequence_view_->magnet_start();
    for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
      auto& clips = manipulate_target_clips_[i];
      auto target_track = sequence_->getTrackAt(i + ghost_sequence_view_->track_offset());
      if (!target_track) continue;
      for (auto clip : clips) {
        clip->setTime(clip->start_time() + start_d_time, clip->end_time() + end_d_time, clip->b_time() + b_d_time);
        target_track->addClip(clip);
      }
    }
  }

  delete ghost_sequence_view_;
  ghost_sequence_view_ = nullptr;
  manipulate_target_clips_.clear();
  manipulate_state_ = ManipulateState::IDLE;
}

void SequenceTimelineView::handleDrag(QDragMoveEvent* event) {
  if (manipulate_state_ != ManipulateState::DRAG_AND_DROP) return;
  QPoint d_mouse_pos = event->pos() - manipulate_start_mouse_pos_;
  int dt = scroll_view_.getTimeAmountRelativeToView(d_mouse_pos.x());
  int last_track_index = getTrackIndexAtPoint(last_mouse_pos_);
  int current_track_index = getTrackIndexAtPoint(event->pos());
  if (last_track_index != current_track_index) {
    int d_track_offset = current_track_index - last_track_index;
    ghost_sequence_view_->setTrackOffset(ghost_sequence_view_->track_offset() + d_track_offset);
  }
  ghost_sequence_view_->setTranslation(dt);
  
}

void SequenceTimelineView::handleDrop(QDropEvent* event) {
  if (manipulate_state_ != ManipulateState::DRAG_AND_DROP) return;
  // Add clips
  int start_d_time = ghost_sequence_view_->start_extent() + ghost_sequence_view_->magnet_start() + ghost_sequence_view_->translation() + ghost_sequence_view_->translation_magnet_time();
  int end_d_time = ghost_sequence_view_->end_extent() + ghost_sequence_view_->magnet_end() + ghost_sequence_view_->translation() + ghost_sequence_view_->translation_magnet_time();
  int b_d_time = ghost_sequence_view_->start_extent() + ghost_sequence_view_->magnet_start();
  for (int i = 0; i < manipulate_target_clips_.size(); i ++) {
    auto& clips = manipulate_target_clips_[i];
    auto target_track = sequence_->getTrackAt(i + ghost_sequence_view_->track_offset());
    if (!target_track) continue;
    for (auto clip : clips) {
      clip->setTime(clip->start_time() + start_d_time, clip->end_time() + end_d_time, clip->b_time() + b_d_time);
      target_track->addClip(clip);
    }
  }
  delete ghost_sequence_view_;
  ghost_sequence_view_ = nullptr;
  manipulate_target_clips_.clear();
  manipulate_state_ = ManipulateState::IDLE;
}


void SequenceTimelineView::updateGhostSequenceView() {
  if (ghost_sequence_view_ == nullptr) return;
  ghost_sequence_view_->show();
  ghost_sequence_view_->setGeometry(rect());
  ghost_sequence_view_->raise();
}

void SequenceTimelineView::blurAllClips() {
  for (auto track_view :track_views_)
    track_view->blurAllClips();
}

int SequenceTimelineView::getTrackIndexAtPoint(QPoint& point) const {
  int local_y = /* mapFromGlobal(point).y(); */ point.y();
  int res = -1;
  int track_start_y = 0;
  for (int i = 0; i < track_views_.size(); i ++) {
    if (local_y < track_start_y) break;
    track_start_y += 30; 
    res = i;
  }
  return res;
}

int SequenceTimelineView::getTrackIndexAtPoint(QPoint&& point) const {
  return getTrackIndexAtPoint(point);
}

void SequenceTimelineView::resizeEvent(QResizeEvent* event) {
  for (int i = 0; i < track_views_.size(); i ++) {
    auto& track_view = track_views_[i];
    track_view->resize(width(), 29);
    track_view->move(0, i * 30);
  }
  updateGhostSequenceView();

  QWidget::resizeEvent(event);
}

void SequenceTimelineView::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  p.setPen(theme.surfaceBrightColor());
  // Draw track border
  p.drawLine(0, 0, width(), 0);
  for (int i = 0; i < track_views_.size(); i ++) {
    auto& track_view = track_views_[i];
    p.drawLine(0, (i + 1) * 30 - 1, width(), (i + 1) * 30 - 1);
  }
  QWidget::paintEvent(event);
}

SequenceScrollView* SequenceTimelineView::scroll_view() {
  return &scroll_view_;
}

const std::set<ClipView*>& SequenceTimelineView::focused_clip_views() {
  return focused_clip_views_;
}

}

}