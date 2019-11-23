#ifndef NLIVE_ACTION_BAR_H_
#define NLIVE_ACTION_BAR_H_

#include <QAction>
#include <QEvent>
#include <QPushButton>
#include <QMouseEvent>
#include "base/common/memory.h"
#include <vector>
#include "base/layout/div.h"
#include "base/ui/svg_sprite.h"
#include "platform/theme/themeservice.h"

namespace nlive {

namespace {

class ActionBarItemView : public QPushButton {

private:
  QAction* action_;
  bool pressed_;

  SvgSprite* normal_sprite_;
  SvgSprite* highlighted_sprite_;
  SvgSprite* disabled_sprite_;

protected:
  void paintEvent(QPaintEvent* e) override;

public:
  ActionBarItemView(QWidget* parent, QAction* action, QString svg_path,
    sptr<IThemeService> theme_service);

  void setSize(int width, int height);

};

}

class ActionBar : public Div {

private:
  sptr<IThemeService> theme_service_;

  std::vector<ActionBarItemView*> items_;
  QSize icon_size_;

  void doLayout();

protected:
  void contentRectUpdated() override;

public:
  ActionBar(QWidget* parent, sptr<IThemeService> theme_service);

  void setIconSize(QSize size);
  void addAction(QAction* action, QString svg_path);

};

}

#endif