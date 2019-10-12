#ifndef NLIVE_EFFECT_CONTROL_PROPERTY_TIMELINE_VIEW_H_
#define NLIVE_EFFECT_CONTROL_PROPERTY_TIMELINE_VIEW_H_

#include <QWidget>
#include <QPainter>
#include "browser/widgets/effect_control/effect_control_layout.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/property.h"

namespace nlive {

namespace effect_control {

template <class T>
class PropertyTimelineView : public QWidget {

private:

protected:
  void paintEvent(QPaintEvent* e) override {
    QPainter p(this);
    p.fillRect(rect(), Qt::lightGray);
  }

public:
  PropertyTimelineView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout_params,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Property<T>> property) :
  QWidget(parent) {
  
}  

};

}

}

#endif