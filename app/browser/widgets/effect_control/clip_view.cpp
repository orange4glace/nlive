#include "browser/widgets/effect_control/clip_view.h"

#include <QDebug>
#include <QEvent>
#include <QPainter>

#include "platform/theme/themeservice.h"
#include "model/effect/effect.h"
#include "model/sequence/clip.h"
#include "browser/widgets/effect_control/transform/transform_effect_view.h"
#include "browser/widgets/effect_control/effect_view_factory.h"
#include "browser/widgets/effect_control/effect_view.h"
#include "platform/logger/logger.h"

namespace nlive {

namespace effect_control {

ClipView::ClipView(
  QWidget* parent,
  QSharedPointer<EffectControlLayout> layout_params,
  QSharedPointer<Sequence> sequence,
  QSharedPointer<Clip> clip,
  SequenceScrollView* sequence_scroll_view,
  QSharedPointer<IThemeService> theme_service) :
  QWidget(parent), theme_service_(theme_service), layout_params_(layout_params),
  sequence_(sequence), clip_(clip), sequence_scroll_view_(sequence_scroll_view) {
  {
    auto& effects = clip->effects();
    int idx = 0;
    for (auto& effect : effects) {
      addEffectView(effect, idx++);
    }
  }
}

void ClipView::addEffectView(QSharedPointer<effect::Effect> effect, int index) {
  auto factory = EffectViewFactoryRegistry::getFactory(effect->type());
  if (!factory) {
    spdlog::get(LOGGER_DEFAULT)->warn("[EffectControl_ClipView] EffectViewFactory not found! expected factory type = {}", effect->type());
    return;
  }
  auto view = factory->create(this, layout_params_, sequence_,
    clip_, effect, sequence_scroll_view_, theme_service_);
  if (!view) {
    spdlog::get(LOGGER_DEFAULT)->warn("[EffectControl_ClipView] View is null! item type = {}", effect->type());
    return;
  }
  auto pair = make_pair(effect, view);
  effect_views_.insert(effect_views_.begin() + index, pair);
  view->show();
}

void ClipView::doLayout() {
  int y = 0;
  for (auto effect_view : effect_views_) {
    effect_view.second->setGeometry(0, y, width(), effect_view.second->sizeHint().height());
    y += effect_view.second->sizeHint().height();
  }
  resize(width(), y);
}

bool ClipView::event(QEvent* event) {
  switch(event->type()) {
  case QEvent::LayoutRequest:
  case QEvent::Resize:
    doLayout();
    return true;
  case QEvent::Paint:
    QPainter p(this);
    p.fillRect(rect(), Qt::darkGreen);
    return true;
  }
  return false;
}

QSize ClipView::sizeHint() const {
  int height = 0;
  for (auto effect_view : effect_views_)
    height += effect_view.second->sizeHint().height();
  return QSize(width(), height);
}

}

}