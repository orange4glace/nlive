#ifndef NLIVE_TIMELINE_WIDGET_H_
#define NLIVE_TIMELINE_WIDGET_H_

#include <QDockWidget>
#include "base/common/memory.h"
#include <QLayout>

#include "browser/widgets/widget.h"
#include "base/layout/fillparentbox.h"
#include "base/common/sig.h"
#include "browser/services/play/play_service.h"

namespace nlive {

class IThemeService;
class ITimelineWidgetService;
class Sequence;

namespace timeline_widget {

class SequenceView;

class TimelineWidget : public QDockWidget, public Widget, public Sig {
  Q_OBJECT;
  
public:
  static const std::string TYPE;

private:
  sptr<IThemeService> theme_service_;
  sptr<PlayService> play_service_;
  sptr<ITimelineWidgetService> timeline_widget_service_;

  sptr<Sequence> sequence_;
  SequenceView* sequence_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;
  void focusInEvent(QFocusEvent* event) override;

public:
  TimelineWidget(
    QWidget* parent,
    sptr<IThemeService> themeService,
    sptr<ITimelineWidgetService> timeline_widget_service,
    sptr<PlayService> play_service);

  void setSequence(sptr<Sequence> sequence);

  sptr<Sequence> sequence();
  SequenceView* sequence_view();

  inline std::string name() const override { return TYPE; }
  
  sig2_t<void (sptr<Sequence> sequence)> onDidChangeSequence;

};

}

}

#endif