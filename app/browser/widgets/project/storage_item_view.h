#ifndef NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_
#define NLIVE_PROJECT_WIDGET_STORAGE_ITEM_VIEW_H_

#include <QWidget>

#include "base/layout/div.h"
#include "base/layout/flex_layout.h"
#include "base/ui/text_box.h"
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




class StorageItemContentViewContainer;
class StorageItemContentView : public Div {
friend class StorageItemContentViewContainer;

protected:
  virtual void onScrubStart();
  virtual void onScrub(double x);
  virtual void onScrubEnd();

public:
  inline StorageItemContentView(QWidget* parent) : Div(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
  }

};





class StorageItemContentViewContainer : public Div {

private:
  ScrubBar* scrub_bar_;
  bool scrubbing_;

  StorageItemContentView* content_view_;

  void _onScrubStart();
  void _onScrub(double x);
  void _onScrubEnd();

  void doLayout();

protected:
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<StorageItem> storage_item_;

  void contentRectUpdated() override;
  void enterEvent(QEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void leaveEvent(QEvent* event) override;

public:
  StorageItemContentViewContainer(QWidget* parent, QSharedPointer<IThemeService> theme_service);
  void setContentView(StorageItemContentView* content_view);


};





class StorageItemTitleView : public FlexLayout {

private:
  QSharedPointer<IThemeService> theme_service_;
  TextBox* left_label_box_;
  TextBox* right_label_box_;

public:
  StorageItemTitleView(QWidget* parent, QSharedPointer<IThemeService> theme_service);

  inline TextBox* left_label_box() { return left_label_box_; }
  inline TextBox* right_label_box() { return right_label_box_; }

};





class StorageItemView : public Div {

private:
  StorageItemContentViewContainer* content_view_;
  StorageItemTitleView* title_view_;

protected:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<StorageItem> storage_item_;

  void contentRectUpdated() override;
  void mousePressEvent(QMouseEvent* event) override;

public:
  StorageItemView(QWidget* parent, QSharedPointer<StorageItem>, QSharedPointer<IThemeService> theme_service);

  QSharedPointer<StorageItem> storage_item();
  inline StorageItemContentViewContainer* content_view() { return content_view_; }
  inline StorageItemTitleView* title_view() { return title_view_; }
  
};

}

}

#endif