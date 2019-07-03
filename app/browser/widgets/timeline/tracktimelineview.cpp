#include "browser/widgets/timeline/tracktimelineview.h"

#include <QPainter>

#include "model/sequence/track.h"
#include "model/sequence/clip.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"

#include <iostream>

namespace nlive {

namespace timelinewidget {

TrackTimelineView::TrackTimelineView(
  QWidget* const parent,
  Track* const track,
  SequenceScrollView* const scrollView,
  IThemeService* const theme_service) : 
  QWidget(parent), track_(track), scrollView_(scrollView), theme_service_(theme_service) {

  auto& clips = track->clips();
  for (auto& clip : clips) handleDidAddClip(clip);
  QObject::connect(track, &Track::onDidAddClip, this, &TrackTimelineView::handleDidAddClip);
  QObject::connect(track, &Track::onWillRemoveClip, this, &TrackTimelineView::handleWillRemoveClip);

}

void TrackTimelineView::handleDidAddClip(Clip* const clip) {
  // Create and add Clip view
  auto view = new TimelineWidgetClipView(this, clip, scrollView_, theme_service_);
  clipViews_.emplace(clip->id(), view);
}

void TrackTimelineView::handleWillRemoveClip(Clip* const clip) {
  // Delete Track view
  clipViews_.erase(clip->id());
}

void TrackTimelineView::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  // p.fillRect(0, 0, width(), height(), theme.primaryColor());
  
  QWidget::paintEvent(event);
}

}

}