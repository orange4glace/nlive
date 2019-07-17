#ifndef _NLIVE_TIMELINE_WIDGET_H_
#define _NLIVE_TIMELINE_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include <QLayout>

#include "base/layout/fillparentview.h"

namespace nlive {

class IThemeService;
class ITimelineWidgetService;
class Sequence;

namespace timelinewidget {

class SequenceView;

class TimelineWidget : public QDockWidget {
  Q_OBJECT;

private:
  IThemeService* theme_service_;
  QSharedPointer<ITimelineWidgetService> timeline_widget_service_;

  QSharedPointer<Sequence> sequence_;
  SequenceView* sequence_view_;

  FillParentView split_left_view_;
  FillParentView split_right_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;
  void focusInEvent(QFocusEvent* event) override;

public:
  TimelineWidget(
    QWidget* parent,
    IThemeService* themeService,
    QSharedPointer<ITimelineWidgetService> timeline_widget_service);

  void setSequence(QSharedPointer<Sequence> sequence);

  QSharedPointer<Sequence> sequence();
  SequenceView* sequence_view();
  

signals:
  void onDidChangeSequence(QSharedPointer<Sequence> sequence);

};

}

}

#endif