#ifndef NLIVE_TIMELINE_WIDGET_CLIP_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_CLIP_VIEW_H_

#include <QWidget>
#include <QCursor>
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

  QCursor left_cursor_;
  QCursor right_cursor_;
  
  QWidget left_handle_outer_;
  QWidget left_handle_inner_;
  QWidget right_handle_outer_;
  QWidget right_handle_inner_;
  QWidget bar_handle_;

  bool border_left_;
  bool border_right_;

  bool focused_;

  QSharedPointer<IThemeService> theme_service_;

  void initializeHandles();

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  ClipView(
    QWidget* const parent,
    QSharedPointer<Track> track,
    QSharedPointer<Clip> clip,
    SequenceScrollView* const scrollView,
    QSharedPointer<IThemeService> const themeService);

  ClipViewHandle testHandle(QPoint&& point) const;

  void setLayout(int outer_left_handle_width, int inner_left_handle_width,
    int handle_width, int inner_right_handle_width, int outer_right_handle_width,
    bool border_left, bool border_right);

  inline QWidget* outer_left_handle() { return &left_handle_outer_; }
  inline QWidget* outer_right_handle() { return &right_handle_outer_; }

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