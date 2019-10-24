#ifndef _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_
#define _NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_

#include <QWidget>

#include "platform/theme/themeservice.h"

namespace nlive {

class StorageItem;

namespace project_widget {

namespace {

class ScrubBar : public QWidget {

private:
  QSharedPointer<IThemeService> theme_service_;
  
  double value_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  ScrubBar(QWidget* parent, QSharedPointer<IThemeService> theme_service);
  void setValue(double x);

};

}

class StorageItemView : public QWidget {

private:
  QColor col_;

  ScrubBar* scrub_bar_;
  bool scrubbing_;

  void _onScrubStart();
  void _onScrub(double x);
  void _onScrubEnd();

protected:
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<StorageItem> storage_item_;

  virtual void onScrubStart();
  virtual void onScrub(double x);
  virtual void onScrubEnd();

  void resizeEvent(QResizeEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void enterEvent(QEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void leaveEvent(QEvent* event) override;

public:
  StorageItemView(QWidget* parent, QSharedPointer<StorageItem>, QSharedPointer<IThemeService> theme_service);

  QSharedPointer<StorageItem> storage_item();
  
};

}

}

#endif