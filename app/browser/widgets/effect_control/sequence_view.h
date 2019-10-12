#ifndef NLIVE_EFFECT_CONTROL_SEQUENCE_VIEW_H_
#define NLIVE_EFFECT_CONTROL_SEQUENCE_VIEW_H_

#include <QWidget>
#include <QSharedPointer>
#include <vector>
#include "base/common/sig.h"

namespace nlive {

class IThemeService;
class Clip;

namespace timelinewidget {

class SequenceView;

}

namespace effect_control {

class EffectControlLayout;
class ClipView;

class SequenceView : public QWidget, public Sig {
  Q_OBJECT

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<EffectControlLayout> layout_;
  timelinewidget::SequenceView* timeline_widget_sequence_view_;

  QSharedPointer<Clip> clip_;
  ClipView* clip_view_;

  void handleDidChangeFocusedClips();
  void createClipView(QSharedPointer<Clip> clip);
  void clearClipView();

protected:
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

public:
  SequenceView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout,
    timelinewidget::SequenceView* timeline_widget_sequence_view,
    QSharedPointer<IThemeService> theme_service);

};

}

}

#endif