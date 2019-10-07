#include "browser/widgets/effect_control/effect_view.h"

#include <QEvent>
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/effect.h"
#include "browser/widgets/effect_control/effect_control_layout.h"

namespace nlive {

namespace effect_control {

EffectView::EffectView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect,
    QSharedPointer<IThemeService> theme_service) :
  theme_service_(theme_service),
  QWidget(parent),
  layout_(layout), clip_(clip) {

}

void EffectView::insertPropertyView(QWidget* view, int index) {
  Q_ASSERT(property_views_.size() >= index && index >= 0);
  property_views_.insert(property_views_.begin() + index, view);
  view->show();
  updateGeometry();
}

void EffectView::doLayout() {
  int y = 0;
  for (auto property_view : property_views_) {
    property_view->move(0, y);
    QSize size_hint = property_view->sizeHint();
    property_view->resize(width(), size_hint.height());
    y += property_view->sizeHint().height();
  }
}

bool EffectView::event(QEvent* event) {
  switch (event->type()) {
  case QEvent::LayoutRequest:
  case QEvent::Resize:
    doLayout();
    return true;
  break;
  }
  return false;
}

QSize EffectView::sizeHint() const {
  int height = 0;
  for (auto property_view : property_views_)
    height += property_view->sizeHint().height();
  return QSize(width(), height);
}

}

}