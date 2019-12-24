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
#include "browser/widgets/timeline/sequenceview.h"

namespace nlive {

namespace effect_control {

template <class T>
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
  
  sptr<EffectControlLayout> layout_params_;
  sptr<Sequence> sequence_;
  sptr<Clip> clip_;
  sptr<effect::Property<T>> property_;

  PropertyFormView<effect::Property<T>>* form_view_;
  PropertyTimelineView<T>* timeline_view_;

  SequenceScrollView* sequence_scroll_view_;

  virtual void updateValue() = 0;

public:
  PropertyView(
    QWidget* widget,
    sptr<EffectControlLayout> layout_params,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Property<T>> property,
    QString label,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service) :
  QWidget(widget), theme_service_(theme_service),
  layout_params_(layout_params), sequence_(sequence), clip_(clip),
  property_(property), sequence_scroll_view_(sequence_scroll_view) {

    form_view_ = new PropertyFormView<effect::Property<T>>(
      this, layout_params, sequence, clip, property, label, theme_service);

    timeline_view_ = new PropertyTimelineView<T>(
      this, layout_params, sequence, clip, property, sequence_scroll_view, theme_service);

    property->onDidUpdate.connect(sig2_t<void (void)>::slot_type(
      &PropertyView<T>::updateValue, this).track(__sig_scope_));
    sequence_scroll_view->onDidUpdate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
      &PropertyView<T>::updateValue, this)));

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

  sptr<effect::Property<T>>* property() {
    return property_;
  }

  PropertyFormView<effect::Property<T>>* form_view() {
    return form_view_;
  }

  PropertyTimelineView<T>* timeline_view() {
     return timeline_view_;
  }

};

}

}

#endif