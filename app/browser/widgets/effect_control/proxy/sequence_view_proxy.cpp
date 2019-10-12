#include "browser/widgets/effect_control/proxy/sequence_view_proxy.h"

#include "browser/widgets/timeline/clipview.h"

namespace nlive {

namespace effect_control {

SequenceViewProxy::SequenceViewProxy(
    timelinewidget::SequenceView* timeline_widget_sequence_view) :
  timeline_widget_sequence_view_(timeline_widget_sequence_view_) {
  timeline_widget_sequence_view_->timeline_view()->onDidChangeFocusedClips.connect(
    sig2_t<void ()>::slot_type(&SequenceViewProxy::handleDidChangeFocusedClips, this).track(__sig_scope_));
}

void SequenceViewProxy::createClipViewProxy(QSharedPointer<Clip> clip) {
  auto clip_view_proxy = QSharedPointer<ClipViewProxy>(
    new ClipViewProxy(sequence_, clip));
  clip_view_proxies_.push_back(clip_view_proxy);
  onDidAddClipViewProxy(clip_view_proxies_.size() - 1, clip_view_proxy);
}

void SequenceViewProxy::clearClipViewProxies() {
  int i = 0;
  for (auto clip_view_proxy : clip_view_proxies_) {
    onWillRemoveClipViewProxy(i, clip_view_proxy);
    i++;
  }
}

void SequenceViewProxy::handleDidChangeFocusedClips() {
  clearClipViewProxies();
  auto clip_views = timeline_widget_sequence_view_->timeline_view()->focused_clip_views();
  for (auto clip_view : clip_views) {
    createClipViewProxy(clip_view->clip());
  }
}

}

}