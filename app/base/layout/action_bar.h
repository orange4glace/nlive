#ifndef NLIVE_ACTION_BAR_H_
#define NLIVE_ACTION_BAR_H_

#include <QAction>
#include <QEvent>
#include <QPushButton>
#include <QMouseEvent>
#include <vector>
#include "base/common/include.h"
#include "base/common/sig.h"
#include "base/layout/div.h"
#include "base/ui/svg_sprite.h"
#include "platform/theme/themeservice.h"

namespace nlive {

class ActionBarItemView : public QPushButton, public Sig {

private:
  sptr<IThemeService> theme_service_;

  sptr<IAction> action_;

  bool pressed_;

  int width_;
  int height_;
  int padding_;

  bool hover_;

protected:
  void paintEvent(QPaintEvent* e) override;

  void enterEvent(QEvent* e) override;
  void leaveEvent(QEvent* e) override;

public:
  ActionBarItemView(QWidget* parent, sptr<IAction> action,
    sptr<IThemeService> theme_service);

  void setSize(int width, int height);
  void setPadding(int padding);

};

class ActionBar : public Div {

private:
  sptr<IThemeService> theme_service_;

  std::vector<ActionBarItemView*> items_;
  QSize icon_size_;
  int icon_padding_;

  void doLayout();

protected:
  void contentRectUpdated() override;

public:
  ActionBar(QWidget* parent, sptr<IThemeService> theme_service);

  void setIconSize(QSize size);
  void setIconPadding(int padding);
  void addAction(sptr<IAction> action);

  const QSize& icon_size() const;
  int icon_padding() const;

  QSize sizeHint() const override;

};

}

#endif