#ifndef NLIVE_EFFECT_CONTROL_SEQUENCE_VIEW_PROXY_H_
#define NLIVE_EFFECT_CONTROL_SEQUENCE_VIEW_PROXY_H_

#include <vector>
#include <QWidget>
#include <QSharedPointer>
#include "base/common/sig.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/effect_control/proxy/clip_view_proxy.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

namespace effect_control {

class SequenceViewProxy : protected Sig {

private:
  timelinewidget::SequenceView* timeline_widget_sequence_view_;
  QSharedPointer<Sequence> sequence_;
  std::vector<QSharedPointer<ClipViewProxy>> clip_view_proxies_;

  void createClipViewProxy(QSharedPointer<Clip> clip);
  void clearClipViewProxies();

  void handleDidChangeFocusedClips();

public:
  SequenceViewProxy(
    timelinewidget::SequenceView* timeline_widget_sequence_view);

  sig2_t<void (size_t, QSharedPointer<ClipViewProxy>)>
      onDidAddClipViewProxy;
  sig2_t<void (size_t, QSharedPointer<ClipViewProxy>)>
      onWillRemoveClipViewProxy;

};

}

}

#endif