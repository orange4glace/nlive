#ifndef NLIVE_TIMELINE_WIDGET_H_
#define NLIVE_TIMELINE_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <QLayout>

#include "base/layout/fillparentbox.h"
#include "base/common/sig.h"
#include "browser/services/play/play_service.h"

namespace nlive {

class IThemeService;
class ITimelineWidgetService;
class Sequence;

namespace timeline_widget {

class SequenceView;

class TimelineWidget : public QDockWidget, public Sig {
  Q_OBJECT;

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<PlayService> play_service_;
  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;

  QSharedPointer<Sequence> sequence_;
  SequenceView* sequence_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;
  void focusInEvent(QFocusEvent* event) override;

public:
  TimelineWidget(
    QWidget* parent,
    QSharedPointer<IThemeService> themeService,
    QSharedPointer<ITimelineWidgetService> timeline_widget_service,
    QSharedPointer<PlayService> play_service);

  void setSequence(QSharedPointer<Sequence> sequence);

  QSharedPointer<Sequence> sequence();
  SequenceView* sequence_view();
  
  sig2_t<void (QSharedPointer<Sequence> sequence)> onDidChangeSequence;

};

}

}

#endif