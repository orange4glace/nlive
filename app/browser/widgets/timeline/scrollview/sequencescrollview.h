#ifndef _NLIVE_SEQUENCE_SCROLL_VIEW_H_
#define _NLIVE_SEQUENCE_SCROLL_VIEW_H_

#include <QWidget>
#include "browser/widgets/timeline/scrollview/scrollbar.h"

namespace nlive {

class Sequence;
class IThemeService;

class SequenceScrollView : public QWidget {
  Q_OBJECT

private:
  class Indicator : public QWidget {
    private:
      IThemeService* theme_service_;
    protected:
      void paintEvent(QPaintEvent* event) override;
    public:
      Indicator(QWidget* parent, IThemeService* theme_service);
  };

  IThemeService* theme_service_;

  QSharedPointer<Sequence> sequence_;

  QWidget* content_widget_;
  SequenceScrollViewScrollbar scrollbar_;

  Indicator indicator_;

  int start_time_;
  int end_time_;
  qreal px_per_frame_;
  qreal unit_frame_time_;
  qreal unit_width_;

  void doUpdate();
  void doResize();

protected:
  void mouseMoveEvent(QMouseEvent* event) override;

  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

public:
  SequenceScrollView(
    QWidget* const parent,
    QWidget* content_widget,
    QSharedPointer<Sequence> sequence,
    IThemeService* theme_service);

  void setContentWidget(QWidget* widget);

  void update(qreal scroll_start, qreal scroll_end);

  int getTimeRelativeToView(int pixel) const;
  int getTimeAmountRelativeToView(int pixel) const;
  int getPositionRelativeToView(int time) const;
  int getPixelAmountRelativeToView(int time) const;

  QWidget* content_widget() const;
  int start_time() const;
  int end_time() const;
  qreal px_per_frame() const;
  qreal unit_frame_time() const;
  qreal unit_width() const;

signals:
  void onDidUpdate();

};

}

#endif