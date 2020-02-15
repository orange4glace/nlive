#ifndef NLIVE_EFFECT_CONTROL_PROPERTY_VIEW_H_
#define NLIVE_EFFECT_CONTROL_PROPERTY_VIEW_H_

#include <QWidget>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "base/layout/div.h"
#include "model/effect/property.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "browser/widgets/effect_control/property/property_form_view.h"
#include "browser/widgets/effect_control/property/property_timeline_view.h"
#include "browser/widgets/effect_control/keyframes_controller.h"
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

namespace effect_control {

class PropertyView : public QWidget, public Sig {

private:
  void doLayout() {
    int timeline_width = layout_params_->timeline_width();
    int form_width = width() - timeline_width;
    form_view_->setGeometry(0, 0, form_width, height());
    timeline_view_->setGeometry(form_width, 0, timeline_width, height());
  }

protected:
  sptr<IThemeService> theme_service_;
  
  sptr<IKeyframesController> keyframes_controller_;
  sptr<EffectControlLayout> layout_params_;
  sptr<Sequence> sequence_;
  sptr<Clip> clip_;
  sptr<effect::IProperty> property_;

  PropertyFormView* form_view_;
  PropertyTimelineView* timeline_view_;

  SequenceScrollView* sequence_scroll_view_;

  virtual void updateValue() = 0;

public:
  PropertyView(
    QWidget* widget,
    sptr<IKeyframesController> keyframes_controller,
    sptr<EffectControlLayout> layout_params,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::IProperty> property,
    QString label,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service) :
  QWidget(widget), theme_service_(theme_service), keyframes_controller_(keyframes_controller),
  layout_params_(layout_params), sequence_(sequence), clip_(clip),
  property_(property), sequence_scroll_view_(sequence_scroll_view) {

    form_view_ = new PropertyFormView(
      this, layout_params, sequence, clip, property, label, theme_service);

    timeline_view_ = new PropertyTimelineView(
      this, keyframes_controller, layout_params, sequence, clip, property, sequence_scroll_view, theme_service);

    property->onDidUpdate.connect(sig2_t<void (void)>::slot_type(
      &PropertyView::updateValue, this).track(__sig_scope_));
    property->onDidInvalidate.connect(sig2_t<void (sptr<effect::IKeyframe>)>::slot_type(
      [this](sptr<effect::IKeyframe>) {
        updateValue();
      }).track(__sig_scope_));
    sequence_scroll_view->onDidUpdate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
      &PropertyView::updateValue, this)));

    layout_params_->onDidUpdate.connect(SIG2_TRACK(sig2_t<void()>::slot_type(
      [this](){ doLayout(); }
    )));
  }

  bool event(QEvent* event) override {
    switch (event->type()) {
    case QEvent::LayoutRequest:
    case QEvent::Resize:
      doLayout();
      return true;
    case QEvent::Paint:
      QPainter p(this);
      return true;
    }
    return false;
  }

  QSize sizeHint() const override {
    return QSize(-1, 20);
  }

  sptr<effect::IProperty> property() {
    return property_;
  }

  PropertyFormView* form_view() {
    return form_view_;
  }

  PropertyTimelineView* timeline_view() {
     return timeline_view_;
  }

};

template <class T>
class PropertyViewTpl : public PropertyView {

protected:
  sptr<effect::Property<T>> property_;

  PropertyViewTpl(
    QWidget* parent,
    sptr<IKeyframesController> keyframes_controller,
    sptr<EffectControlLayout> layout,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Property<T>> property,
    QString label,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service) :
    PropertyView(parent, keyframes_controller, layout, sequence, clip, property, label,
      sequence_scroll_view, theme_service), property_(property) {

  }

};

}

}

#endif