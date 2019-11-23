#include "base/layout/action_bar.h"

#include <QDebug>
#include <QPainter>

namespace nlive {

namespace {
enum ActionBarItemViewState { Unchecked, Checked, Pressed, };
}

ActionBarItemView::ActionBarItemView(QWidget* parent, QAction* action,
  QString svg_path, sptr<IThemeService> theme_service) :
  QPushButton(parent), action_(action) {

  connect(this, &QPushButton::pressed, [this]() {
    if (action_->isEnabled()) {
      action_->trigger();
    }
  });
  connect(this, &QPushButton::clicked, [this]() {
    pressed_ = true;
    update();
  });
  connect(action, &QAction::changed, [this]() {
    update();
  });

  auto theme = theme_service->getTheme();
  normal_sprite_ = new SvgSprite(svg_path);
  normal_sprite_->setColor(theme.buttonEnabled());
  highlighted_sprite_ = new SvgSprite(svg_path);
  highlighted_sprite_->setColor(theme.buttonHighlighted());
  disabled_sprite_ = new SvgSprite(svg_path);
  disabled_sprite_->setColor(theme.buttonDisabled());
}

void ActionBarItemView::setSize(int width, int height) {
  normal_sprite_->resize(width, height);
  highlighted_sprite_->resize(width, height);
  disabled_sprite_->resize(width, height);
}

void ActionBarItemView::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  ActionBarItemViewState state;
  if (!action_->isEnabled()) {
    p.drawPixmap(rect(), disabled_sprite_->pixmap());
    return;
  }
  if (pressed_) state = action_->isChecked() ? Unchecked : Checked;
  else state = action_->isChecked() ? Checked : Unchecked;
  if (state == Checked) {
    p.drawPixmap(rect(), highlighted_sprite_->pixmap());
  }
  else {
    p.drawPixmap(rect(), normal_sprite_->pixmap());
  }
}

ActionBar::ActionBar(QWidget* parent, sptr<IThemeService> theme_service) :
  Div(parent), theme_service_(theme_service), icon_size_(20, 20) {

}

void ActionBar::contentRectUpdated() {
  doLayout();
}

void ActionBar::doLayout() {
  int x = 0, y = 0;
  for (auto item : items_) {
    setChildGeometry(item, QRect(x, y, icon_size_.width(), icon_size_.height()));
    x += icon_size_.width();
    if (x + icon_size_.width() >= rect().width()) {
      x = 0;
      y += icon_size_.height();
    }
  }
}

void ActionBar::setIconSize(QSize size) {
  icon_size_ = size;
  for (auto item : items_) item->setSize(size.width(), size.height());
  doLayout();
}

void ActionBar::addAction(QAction* action, QString svg_path) {
  ActionBarItemView* item = new ActionBarItemView(this, action, svg_path, theme_service_);
  item->setSize(icon_size_.width(), icon_size_.height());
  items_.push_back(item);
  doLayout();
}

}