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

namespace timelinewidget {

TrackTimelineView::TrackTimelineView(
  QWidget* const parent,
  QSharedPointer<Track> const track,
  SequenceScrollView* const scrollView,
  IThemeService* const theme_service) : 
  QWidget(parent), track_(track), scrollView_(scrollView), theme_service_(theme_service) {

  auto& clips = track->clips();
  for (auto& clip : clips) handleDidAddClip(clip);
  track->onDidAddClip.connect(SIG2_TRACK(sig2_t<void (QSharedPointer<Clip>)>::slot_type(
    boost::bind(&TrackTimelineView::handleDidAddClip, this, _1))));
  track->onWillRemoveClip.connect(SIG2_TRACK(sig2_t<void (QSharedPointer<Clip>)>::slot_type(
    boost::bind(&TrackTimelineView::handleWillRemoveClip, this, _1))));
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
}

void TrackTimelineView::blurAllClips() {
  auto copied_focused_clip_views = focused_clip_views_;
  for (auto clip_view : copied_focused_clip_views) {
    clip_view->blur();
  }
}

void TrackTimelineView::mouseMoveEvent(QMouseEvent* event) {
  event->ignore();
}

void TrackTimelineView::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  // p.fillRect(0, 0, width(), height(), theme.primaryColor());
  
  QWidget::paintEvent(event);
}

const std::set<ClipView*>& TrackTimelineView::clip_views() {
  return clip_views_;
}

const std::set<ClipView*>& TrackTimelineView::focused_clip_views() {
  return focused_clip_views_;
}

}

}