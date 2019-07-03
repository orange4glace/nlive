#include "browser/widgets/timeline/sequencetimelineview.h"

#include "platform/theme/themeservice.h"
#include "model/sequence/track.h"
#include "model/sequence/sequence.h"
#include "browser/widgets/timeline/trackview.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"

#include <QPainter>
#include <QDebug>

namespace nlive {

namespace timelinewidget {

SequenceTimelineView::SequenceTimelineView(
  QWidget* parent,
  Sequence* const sequence,
  IThemeService* const theme_service) :
  QWidget(),
  sequence_(sequence),
  scroll_view_(parent, this, sequence, theme_service),
  theme_service_(theme_service) {
  setParent(&scroll_view_);

  auto tracks = sequence->tracks();
  for (int i = 0; i < tracks.size(); i ++) handleDidAddTrack(tracks[i], i);
  QObject::connect(sequence, &Sequence::onDidAddTrack, this, &SequenceTimelineView::handleDidAddTrack);
}

void SequenceTimelineView::handleDidAddTrack(Track* const track, int index) {
  // Create and add Track view
  auto view = new TrackTimelineView(this, track, &scroll_view_, theme_service_);
  track_views_.emplace(track_views_.begin() + index, view);
}

void SequenceTimelineView::handleWillRemoveTrack(Track* const track, int index) {
  // Delete Track view
  track_views_.erase(track_views_.begin() + index);
}

void SequenceTimelineView::resizeEvent(QResizeEvent* event) {
  for (int i = 0; i < track_views_.size(); i ++) {
    auto& track_view = track_views_[i];
    track_view->resize(width(), 29);
    track_view->move(0, i * 30);
  }

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

}

}