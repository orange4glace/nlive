#ifndef _NLIVE_TIMELINE_WIDGET_CLIP_VIEW_H_
#define _NLIVE_TIMELINE_WIDGET_CLIP_VIEW_H_

#include <QWidget>
#include <QEvent>
#include <QSharedPointer>
#include "base/common/sig.h"

namespace nlive {

class Track;
class Clip;
class SequenceScrollView;
class IThemeService;

namespace timelinewidget {

enum ClipViewHandle {
  LEFT_OUTER,
  LEFT_INNER,
  RIGHT_OUTER,
  RIGHT_INNER,
  BAR,
  NONE
};

class ClipViewHandleMouseEvent : public QEvent {

};

class ClipView : public QWidget, public Sig {
  Q_OBJECT

private:
  QSharedPointer<Track> track_;
  QSharedPointer<Clip> clip_;
  SequenceScrollView* scrollView_;
  
  QWidget left_handle_inner_;
  QWidget left_handle_outer_;
  QWidget right_handle_inner_;
  QWidget right_handle_outer_;
  QWidget bar_handle_;

  bool focused_;

  IThemeService* theme_service_;

  void initializeHandles();
  void updateView();

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  ClipView(
    QWidget* const parent,
    QSharedPointer<Track> track,
    QSharedPointer<Clip> clip,
    SequenceScrollView* const scrollView,
    IThemeService* const themeService);

  ClipViewHandle testHandle(QPoint&& point) const;

  void focus();
  void blur();
  bool focused() const;

  QSharedPointer<Clip> clip();

signals:
  void onDidFocus();
  void onDidBlur();

};

}

}

#endif