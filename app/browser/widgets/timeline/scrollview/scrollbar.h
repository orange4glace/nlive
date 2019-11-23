#ifndef NLIVE_SEQUENCE_SCROLL_VIEW_SCROLLBAR_H_
#define NLIVE_SEQUENCE_SCROLL_VIEW_SCROLLBAR_H_

#include <QWidget>
#include <QPushButton>
#include "base/common/memory.h"

namespace nlive {

class IThemeService;

class SequenceScrollViewScrollbar : public QWidget {
  Q_OBJECT

private:
  sptr<IThemeService> theme_service_;

  qreal start_;
  qreal end_;

  class HandleBar : public QPushButton {
  private:
    sptr<IThemeService> theme_service_;
    SequenceScrollViewScrollbar* scrollbar_;
    QPoint last_mouse_pos_;
  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
  public:
    HandleBar(SequenceScrollViewScrollbar* scrollbar, sptr<IThemeService> theme_service);
  };
  class HandleButton : public QPushButton {
  private:
    sptr<IThemeService> theme_service_;
    SequenceScrollViewScrollbar* scrollbar_;
    bool left_;
    QPoint last_mouse_pos_;
  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
  public:
    HandleButton(HandleBar* handlebar, SequenceScrollViewScrollbar* scrollbar, bool left, sptr<IThemeService> theme_service);
  };

  HandleBar bar_handle_;
  HandleButton left_handle_;
  HandleButton right_handle_;

  void doUpdate(bool from_internal);

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
public:
  SequenceScrollViewScrollbar(QWidget* parent, qreal start, qreal end, sptr<IThemeService> theme_service);

  void setStart(qreal value, bool from_internal = false);
  void setEnd(qreal value, bool from_internal = false);
  void setValue(qreal start, qreal end, bool from_internal = false);

  qreal start() const;
  qreal end() const;

signals:
  void onDidUpdateByUI();
  void onDidUpdate();

};

}

#endif