#ifndef NLIVE_ACTION_BAR_H_
#define NLIVE_ACTION_BAR_H_

#include <QAction>
#include <QEvent>
#include <QPushButton>
#include <QMouseEvent>
#include <vector>
#include <optional>
#include "base/common/include.h"
#include "base/common/sig.h"
#include "base/layout/div.h"
#include "base/ui/svg_sprite.h"
#include "platform/theme/themeservice.h"

namespace nlive {

enum Alignment {
  Auto, Left, Center, Right
};

struct IActionViewItem {
  virtual void render(QWidget* parent) = 0;
};

using IActionViewItemProvider =
    std::function<IActionViewItem* (sptr<IAction> action)>;

struct IActionBarOptions {
  std::optional<IActionViewItemProvider> action_view_item_provider;
};

class BaseActionViewItem : public QPushButton, public IActionViewItem, public Sig {

private:
  int width_;
  int height_;
  int padding_;

protected:
  sptr<IThemeService> theme_service_;
  sptr<IAction> action_;
  
public:
  BaseActionViewItem(QWidget* parent, sptr<IAction> action,
    sptr<IThemeService> theme_service);

  void setSize(int width, int height);
  void setPadding(int padding);
  
  void render(QWidget* parent) override;

};

class ActionBarViewItem : public BaseActionViewItem {

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
  ActionBarViewItem(QWidget* parent, sptr<IAction> action,
    sptr<IThemeService> theme_service);

};

class ActionBar : public Div {

private:
  sptr<IThemeService> theme_service_;
  IActionBarOptions options_;

  std::vector<ActionBarViewItem*> items_;

  QSize icon_size_;
  int icon_padding_;
  Alignment alignment_;

  void doLayout();

protected:
  void contentRectUpdated() override;

public:
  ActionBar(QWidget* parent, std::optional<IActionBarOptions> options,
      sptr<IThemeService> theme_service);

  void setIconSize(QSize size);
  void setIconPadding(int padding);
  void setAlignment(Alignment alignment);
  void addAction(sptr<IAction> action);

  const QSize& icon_size() const;
  int icon_padding() const;
  Alignment alignment() const;

  QSize sizeHint() const override;

};

}

#endif