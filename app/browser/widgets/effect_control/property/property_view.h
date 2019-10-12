#ifndef NLIVE_EFFECT_CONTROL_PROPERTY_VIEW_H_
#define NLIVE_EFFECT_CONTROL_PROPERTY_VIEW_H_

#include <QWidget>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QSharedPointer>
#include "base/common/sig.h"
#include "model/effect/property.h"
#include "platform/theme/themeservice.h"
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "browser/widgets/effect_control/property/property_form_view.h"
#include "browser/widgets/effect_control/property/property_timeline_view.h"

namespace nlive {

namespace effect_control {

template <class T>
class PropertyView : public QWidget, public Sig {

private:
  void doLayout() {
    form_view_->resize(width() / 2, height());
    timeline_view_->resize(width() / 2, height());
    form_view_->move(0, 0);
    timeline_view_->move(width() / 2, 0);
  }

protected:
  QSharedPointer<IThemeService> theme_service_;
  
  QSharedPointer<EffectControlLayout> layout_params_;
  QSharedPointer<Sequence> sequence_;
  QSharedPointer<Clip> clip_;
  QSharedPointer<effect::Property<T>> property_;

  PropertyFormView<effect::Property<T>>* form_view_;
  PropertyTimelineView<T>* timeline_view_;

  virtual void updateValue() = 0;

public:
  PropertyView(
    QWidget* widget,
    QSharedPointer<EffectControlLayout> layout_params,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Property<T>> property,
    QString label,
    QSharedPointer<IThemeService> theme_service) :
  QWidget(widget), theme_service_(theme_service),
  layout_params_(layout_params), sequence_(sequence), clip_(clip),
  property_(property) {
  form_view_ = new PropertyFormView<effect::Property<T>>(
    this, layout_params, sequence, clip, property, label, theme_service);
  timeline_view_ = new PropertyTimelineView<T>(
    this, layout_params, sequence, clip, property);
  }

  bool event(QEvent* event) override {
    switch (event->type()) {
    case QEvent::LayoutRequest:
    case QEvent::Resize:
      doLayout();
      return true;
    case QEvent::Paint:
      QPainter p(this);
      p.fillRect(rect(), Qt::darkRed);
      return true;
    }
    return false;
  }

  QSize sizeHint() const override {
    return QSize(-1, 50);
  }

  QSharedPointer<effect::Property<T>>* property() {
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