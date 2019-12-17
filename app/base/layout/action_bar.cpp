#include "base/layout/action_bar.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QFontDatabase>
#include "base/common/actions.h"

#include <iostream>
namespace nlive {

namespace {
enum ActionBarItemViewState { Unchecked, Checked, Pressed, };
}

ActionBarItemView::ActionBarItemView(QWidget* parent, sptr<IAction> action,
  sptr<IThemeService> theme_service) :
  QPushButton(parent), theme_service_(theme_service), action_(action),
  pressed_(false) {
  setMouseTracking(true);
  width_ = height_ = 0;
  hover_ = false;

  if (auto a = std::dynamic_pointer_cast<Action>(action)) {
    a->onDidChange.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
      [this]() {
        update();
      })));
  }
  connect(this, &QPushButton::pressed, [this]() {
    pressed_ = true;
    update();
  });
  connect(this, &QPushButton::released, [this]() {
    pressed_ = false;
    update();
  });
  connect(this, &QPushButton::clicked, [this]() {
    if (action_->enabled()) {
      action_->run(nullptr);
    }
  });
}

void ActionBarItemView::setSize(int width, int height) {
  width_ = width;
  height_ = height;
}

void ActionBarItemView::setPadding(int padding) {
  padding_ = padding;
  update();
}

void ActionBarItemView::enterEvent(QEvent* e) {
  if (!hover_) {
    hover_ = true;
    update();
  }
}

void ActionBarItemView::leaveEvent(QEvent* e) {
  if (hover_) {
    hover_ = false;
    update();
  }
}

static int FA = -1;
void ActionBarItemView::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  auto theme = theme_service_->getTheme();
  if (pressed_) {
    QPainterPath path;
    path.addRoundedRect(rect(), 3, 3);
    QPen pen(theme.surfaceDarkColor());
    p.setPen(pen);
    p.fillPath(path, theme.surfaceDarkColor());
  }
  else if (hover_) {
    QPainterPath path;
    path.addRoundedRect(rect(), 3, 3);
    QPen pen(theme.surfaceDarkColor());
    p.setPen(pen);
    p.fillPath(path, theme.surfaceDarkColor());
  }
  QRect icon_rect = rect().marginsRemoved(QMargins(padding_, padding_, padding_, padding_));
  QPen pen;
  QFont font;
  font.setPixelSize(width_);
  font.setFamily("Font Awesome 5 Free Solid");
  ActionBarItemViewState state;
  if (!action_->enabled()) {
    pen.setColor(theme.buttonDisabled());
  }
  else {
    // if (pressed_) state = action_->checked() ? Unchecked : Checked;
    // else state = action_->checked() ? Checked : Unchecked;
    // if (state == Checked) {
    //   pen.setColor(theme.buttonHighlighted());
    // }
    // else {
    //   pen.setColor(theme.buttonEnabled());
    // }
    if (pressed_) {
      pen.setColor(theme.buttonHighlighted());
    }
    else if (hover_) {
      pen.setColor(theme.buttonHovered());
    }
    else {
      pen.setColor(theme.buttonEnabled());
    }
  }
  p.setFont(font);
  p.setPen(pen);
  p.drawText(rect(), Qt::AlignCenter, QString::fromStdWString(action_->icon()));
}

ActionBar::ActionBar(QWidget* parent, sptr<IThemeService> theme_service) :
  Div(parent), theme_service_(theme_service), icon_size_(20, 20), icon_padding_(8) {
  if (FA == -1) {
    FA = QFontDatabase::addApplicationFont(":/browser/FontAwesome.otf");
  }
}

void ActionBar::contentRectUpdated() {
  doLayout();
}

void ActionBar::doLayout() {
  int x = 0, y = 0;
  auto r = content_rect();
  int icon_width = icon_size_.width() + icon_padding_ * 2;
  int icon_height = icon_size_.height() + icon_padding_ * 2;
  int total_width = icon_width * items_.size();
  if (alignment_ == Alignment::Auto || alignment_ == Alignment::Left) {
    x = 0;
  }
  else if (alignment_ == Alignment::Center) {
    x = r.width() / 2 - total_width / 2;
  }
  else if (alignment_ == Alignment::Right) {
    x = r.width() - total_width;
  }
  for (auto item : items_) {
    setChildGeometry(item, QRect(x, y, icon_width, icon_height));
    x += icon_width;
    // if (x + icon_width >= rect().width()) {
    //   x = 0;
    //   y += icon_height;
    // }
  }
}

void ActionBar::setIconSize(QSize size) {
  icon_size_ = size;
  for (auto item : items_) item->setSize(size.width(), size.height());
  doLayout();
}

void ActionBar::setIconPadding(int padding) {
  icon_padding_ = padding;
  for (auto item : items_) item->setPadding(padding);
}

void ActionBar::setAlignment(Alignment alignment) {
  alignment_ = alignment;
  doLayout();
}

void ActionBar::addAction(sptr<IAction> action) {
  ActionBarItemView* item = new ActionBarItemView(this, action, theme_service_);
  item->setSize(icon_size_.width(), icon_size_.height());
  item->setPadding(icon_padding_);
  items_.push_back(item);
  doLayout();
}

const QSize& ActionBar::icon_size() const {
  return icon_size_;
}

Alignment ActionBar::alignment() const {
  return alignment_;
}

QSize ActionBar::sizeHint() const {
  return QSize(-1, icon_size_.height() + icon_padding_);
}

}