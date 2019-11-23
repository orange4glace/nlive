#ifndef NLIVE_EFFECT_CONTROL_PROPERTY_FORM_VIEW_H_
#define NLIVE_EFFECT_CONTROL_PROPERTY_FORM_VIEW_H_

#include <QWidget>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "base/ui/text_box.h"
#include "model/effect/property.h"
#include "browser/widgets/effect_control/property/number_input_box.h"
#include "platform/theme/themeservice.h"

namespace nlive {

class Sequence;
class Clip;

namespace effect {

namespace value {

class Vector2;

}

}

namespace effect_control {

class EffectControlLayout;
class NumberInputBox;

template <class T>
class PropertyAnimateToggleButton : public QPushButton, public Sig {

private:
  sptr<IThemeService> theme_service_;

  bool animated_;
  sptr<T> property_;

  void handleClick() {
    property_->setAnimated(!property_->animated());
  }

  void handleDidChangeAnimated(bool value) {
    animated_ = value;
    update();
  }

protected:
  void paintEvent(QPaintEvent* event) override {
    auto& theme = theme_service_->getTheme();
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), 3, 3);
    if (animated_) {
      p.fillPath(path, theme.surfaceBrightColor());
    }
    else {
      p.strokePath(path, theme.surfaceBrightColor());
    }
  }

public:
  PropertyAnimateToggleButton(
    QWidget* parent,
    sptr<EffectControlLayout> layout_params,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<T> property,
    sptr<IThemeService> theme_service) :
  QPushButton(parent), theme_service_(theme_service), property_(property) {
    animated_ = property->animated();

    connect(this, &PropertyAnimateToggleButton::clicked, [this](bool checked) {
      handleClick();
    });
    property->onDidChangeAnimated.connect(
      sig2_t<void (bool)>::slot_type
      (&PropertyAnimateToggleButton<T>::handleDidChangeAnimated, this, _1).track(__sig_scope_));
  }

};

template <class T>
class PropertyFormView : public QWidget, protected Sig {

private:
  QString label_;
  PropertyAnimateToggleButton<T>* animate_toggle_button_;
  TextBox* label_view_;
  std::vector<QWidget*> input_views_;

  void handleDidChangeAnimatable(bool value) {
    if (value) {
      if (animate_toggle_button_ != nullptr) return;
      animate_toggle_button_ = new PropertyAnimateToggleButton<T>(
        this, layout_params_, sequence_, clip_, property_, theme_service_);
      animate_toggle_button_->show();
      doLayout();
    }
    else {
      if (animate_toggle_button_ == nullptr) return;
      delete animate_toggle_button_;
      animate_toggle_button_ = nullptr;
    }

  }

  void doLayout() {
    animate_toggle_button_->move(0, 0);
    animate_toggle_button_->resize(20, 20);
    int value_width = layout_params_->form_value_width();
    int label_width = width() - value_width;
    label_view_->move(20, 0);
    label_view_->resize(label_width - 20, 20);
    int input_view_x = label_width;
    for (int i = 0; i < input_views_.size(); i ++) {
      auto input_view = input_views_[i];
      input_view->move(input_view_x, 0);
      input_view->resize(60, 20);
      input_view_x += 80;
    }
  }
  
  void doPaint() {
    QPainter p(this);
    auto& theme = theme_service_->getTheme();
    p.setPen(theme.surfaceBrightColor());
    p.drawLine(0, height() - 1, width(), height() - 1);
  }

protected:
  sptr<IThemeService> theme_service_;
  sptr<EffectControlLayout> layout_params_;
  sptr<Sequence> sequence_;
  sptr<Clip> clip_;
  sptr<T> property_;

  bool event(QEvent* event) override {
    switch (event->type()) {
    case QEvent::LayoutRequest:
    case QEvent::Resize:
      doLayout();
      return true;
    case QEvent::Paint:
      doPaint();
      return true;
    }
    return false;
  }

public:
  PropertyFormView(
    QWidget* parent,
    sptr<EffectControlLayout> layout_params,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<T> property,
    QString label,
    sptr<IThemeService> theme_service) :
    theme_service_(theme_service),
    QWidget(parent), layout_params_(layout_params),
    sequence_(sequence), clip_(clip), property_(property),
    label_(label), animate_toggle_button_(nullptr) {
    label_view_ = new TextBox(this, label, Qt::AlignVCenter);
    label_view_->setColor(theme_service->getTheme().surfaceTextColor())->setPadding(Div::LEFT, 10);
    theme_service->onDidUpdate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type([this]() {
      label_view_->setColor(theme_service_->getTheme().surfaceTextColor())->setPadding(Div::LEFT, 10);
    })));
    handleDidChangeAnimatable(property->animatable());
    property->onDidChangeAnimatable.connect(
      sig2_t<void (bool)>::slot_type
      (&PropertyFormView<T>::handleDidChangeAnimatable, this, _1).track(__sig_scope_));
    layout_params_->onDidUpdate.connect(SIG2_TRACK(sig2_t<void()>::slot_type(
      [this]() { doLayout(); }
    )));
  }

  void addInputView(QWidget* widget) {
    input_views_.push_back(widget);
    widget->setParent(this);
    widget->show();
    doLayout();
  }

  QSize sizeHint() const override {
    return QSize(width(), 20);
  }

};

}

}

#endif