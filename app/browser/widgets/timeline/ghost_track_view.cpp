#include "browser/widgets/timeline/ghost_track_view.h"

#include <QDebug>
#include <QPainter>

#include "browser/widgets/timeline/ghost_clip_view.h"
#include "browser/widgets/timeline/scrollview/sequencescrollview.h"


namespace nlive {

namespace timeline_widget {

GhostTrackView::GhostTrackView(QWidget* parent, SequenceScrollView* scroll_view) :
  QWidget(parent),
  scroll_view_(scroll_view) {
}

void GhostTrackView::addGhostClipView(GhostClipView* view) {
  view->setParent(this);
  view->show();
  ghost_clip_views_.insert(view);
  emit onDidAddGhostClipView(view);
}

void GhostTrackView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), QColor(30, 80 ,20, 125));
}

void GhostTrackView::resizeEvent(QResizeEvent* event) {
  for (auto clip : ghost_clip_views_) {
    clip->resize(0, height());
  }
}

const std::set<GhostClipView*>& GhostTrackView::ghost_clip_views() {
  return ghost_clip_views_;
}

}

}