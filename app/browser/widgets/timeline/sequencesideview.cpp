#include "browser/widgets/timeline/sequencesideview.h"

#include "model/sequence/sequence.h"
#include "model/sequence/track.h"
#include "platform/theme/themeservice.h"

#include "browser/widgets/timeline/tracksideview.h"

#include <QPainter>

namespace nlive {

namespace timeline_widget {

SequenceSideView::SequenceSideView(
  QWidget* parent,
  sptr<Sequence> sequence,
  sptr<IThemeService> theme_service) : 
  Div(parent), sequence_(sequence), theme_service_(theme_service) {

  auto& tracks = sequence->tracks();
  for (int i = 0; i < tracks.size(); i ++) addTrackView(tracks[i], i);
  
  sequence->onDidAddTrack.connect(sig2_t<void (sptr<Track>, int)>::slot_type(
    [this](sptr<Track> track, int index) {
      this->addTrackView(track, index);
    }).track(__sig_scope_));
  sequence->onWillRemoveTrack.connect(sig2_t<void (sptr<Track>, int)>::slot_type(
    [this](sptr<Track> track, int index) {
      this->removeTrackView(track, index);
    }).track(__sig_scope_));

}

void SequenceSideView::addTrackView(sptr<Track> track, int index) {
  TrackSideView* view = new TrackSideView(this, track, theme_service_);
  track_views_.emplace(track_views_.begin() + index, view);
}

void SequenceSideView::removeTrackView(sptr<Track> track, int index) {
  auto view = track_views_[index];
  delete view;
}

void SequenceSideView::contentRectUpdated() {
  // Space for ruler
  // TODO : remove hard coding
  for (int i = 0; i < track_views_.size(); i ++) {
    auto& track_view = track_views_[i];
    track_view->resize(width(), 29);
    track_view->move(0, i * 30 + 30);
  }
}

void SequenceSideView::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  p.setPen(theme.surfaceBrightColor());
  // Draw track border
  p.drawLine(0, 0, width(), 0);
  for (int i = 0; i < track_views_.size(); i ++) {
    auto& track_view = track_views_[i];
    p.drawLine(0, (i + 1) * 30 - 1, width(), (i + 1) * 30 - 1);
  }
  p.drawLine(width() - 1, 0, width() - 1, height());
}

}

}