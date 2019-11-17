#include "browser/widgets/timeline/tracktimelineview.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <iostream>

#include "model/sequence/track.h"
#include "model/sequence/clip.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"

namespace nlive {

namespace timeline_widget {

TrackTimelineView::TrackTimelineView(
  QWidget* const parent,
  QSharedPointer<Track> const track,
  SequenceScrollView* const scrollView,
  QSharedPointer<IThemeService> const theme_service) : 
  QWidget(parent), track_(track), scrollView_(scrollView), theme_service_(theme_service) {

  auto& clips = track->clips();
  for (auto& clip : clips) handleDidAddClip(clip);
  track->onDidAddClip.connect(SIG2_TRACK(sig2_t<void (QSharedPointer<Clip>)>::slot_type(
    boost::bind(&TrackTimelineView::handleDidAddClip, this, _1))));
  track->onWillRemoveClip.connect(SIG2_TRACK(sig2_t<void (QSharedPointer<Clip>)>::slot_type(
    boost::bind(&TrackTimelineView::handleWillRemoveClip, this, _1))));
  track->onDidChangeClipTime.connect(SIG2_TRACK(sig2_t<void (QSharedPointer<Clip>, int, int, int)>::slot_type(
    [this](QSharedPointer<Clip>, int, int, int) {
      updateClipViews();
  })));
}

void TrackTimelineView::handleDidAddClip(QSharedPointer<Clip> clip) {
  // Create and add Clip view
  auto view = new ClipView(this, track_, clip, scrollView_, theme_service_);
  QObject::connect(view, &ClipView::onDidFocus, this, [this, view]() {
    focused_clip_views_.insert(view);
    emit onDidFocusClip(view);
  });
  QObject::connect(view, &ClipView::onDidBlur, this, [this, view]() {
    focused_clip_views_.erase(view);
    emit onDidBlurClip(view);
  });
  clip_views_.emplace(view);
  clip_to_view_map_.insert({ clip, view });
  view->show();
  updateClipViews();
}

void TrackTimelineView::handleWillRemoveClip(QSharedPointer<Clip> clip) {
  // Delete Track view
  auto it = clip_to_view_map_.find(clip);
  Q_ASSERT(it != clip_to_view_map_.end());
  auto view = it->second;
  view->blur();
  focused_clip_views_.erase(view);
  clip_views_.erase(view);
  clip_to_view_map_.erase(clip);
  delete view;
  updateClipViews();
}

void TrackTimelineView::doLayout() {
  for (auto clip_view : clip_views_) {
    auto clip = clip_view->clip();
    int start_pos = scrollView_->getPositionRelativeToView(clip->start_time());
    int end_pos = scrollView_->getPositionRelativeToView(clip->end_time());
    clip_view->setGeometry(start_pos - clip_view->outer_left_handle()->width(),
      0, end_pos - start_pos + clip_view->outer_left_handle()->width() + clip_view->outer_right_handle()->width(), height());
  }
}

void TrackTimelineView::updateClipViews() {
  int prev_clip_end_pos = -1e8;
  
  int MAX_HANDLE_WIDTH = 20;
  int MIN_SPACE_BETWEEN_CLIPS = 10;
  int MIN_CLIP_HANDLE_WIDTH = 10;
  for (auto it = clip_to_view_map_.begin(); it != clip_to_view_map_.end(); it++) {
    auto& p = *it;
    auto& clip_view = p.second;
    auto clip = clip_view->clip();
    int start_pos = scrollView_->getPositionRelativeToView(clip->start_time());
    int end_pos = scrollView_->getPositionRelativeToView(clip->end_time());
    int next_clip_start_pos = end_pos + 1000;
    auto next_it = std::next(it);
    if (next_it != clip_to_view_map_.end()) {
      auto next_clip = (*next_it).first;
      next_clip_start_pos = scrollView_->getPositionRelativeToView(
        next_clip->start_time());
    }
    int handle_width = end_pos - start_pos;
    int outer_left_handle_width = std::min(MAX_HANDLE_WIDTH,
        std::max(0, (start_pos - prev_clip_end_pos - MIN_SPACE_BETWEEN_CLIPS) / 2));
    int inner_left_handle_width, inner_right_handle_width;
    inner_left_handle_width = inner_right_handle_width = std::min(MAX_HANDLE_WIDTH,
        std::max(0, (handle_width - MAX_HANDLE_WIDTH) / 2));
    int outer_right_handle_width = std::min(MAX_HANDLE_WIDTH,
        std::max(0, (next_clip_start_pos - end_pos - MIN_SPACE_BETWEEN_CLIPS) / 2));
    clip_view->setLayout(outer_left_handle_width, inner_left_handle_width, handle_width,
      inner_right_handle_width, outer_right_handle_width,
      false, (next_clip_start_pos - end_pos == 0));
    prev_clip_end_pos = end_pos;
  }
}

ClipView* const TrackTimelineView::getClipView(QSharedPointer<Clip> clip) {
  Q_ASSERT(clip_to_view_map_.count(clip));
  return clip_to_view_map_[clip];
}

void TrackTimelineView::blurAllClips() {
  auto copied_focused_clip_views = focused_clip_views_;
  for (auto clip_view : copied_focused_clip_views) {
    clip_view->blur();
  }
}

bool TrackTimelineView::event(QEvent* event) {
  switch (event->type()) {
  case QEvent::MouseMove:
    event->ignore();
    return true;
  case QEvent::LayoutRequest:
    doLayout();
    return true;
  }
  return false;
}

const std::set<ClipView*>& TrackTimelineView::clip_views() {
  return clip_views_;
}

const std::set<ClipView*>& TrackTimelineView::focused_clip_views() {
  return focused_clip_views_;
}

}

}