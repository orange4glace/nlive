#ifndef _NLIVE_TIMELINE_WIDGET_H_
#define _NLIVE_TIMELINE_WIDGET_H_

#include <QDockWidget>
#include <QLayout>

#include "base/layout/fillparentview.h"

namespace nlive {

class IThemeService;
class Sequence;

namespace timelinewidget {

class SequenceView;

class TimelineWidget : public QDockWidget {
  Q_OBJECT;

private:
  IThemeService* theme_service_;

  Sequence* sequence_;
  SequenceView* sequence_view_;

  FillParentView split_left_view_;
  FillParentView split_right_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  TimelineWidget(QWidget* parent, IThemeService* themeService);

  void setSequence(Sequence* sequence);

};

}

}

#endif