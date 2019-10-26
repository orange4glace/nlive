#include "browser/widgets/effect_control/effect_view.h"

#include <QEvent>
#include <QPainter>
#include "base/common/perf.h"
#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "model/effect/effect.h"
#include "browser/widgets/effect_control/effect_control_layout.h"

namespace nlive {

namespace effect_control {

EffectViewHeader::EffectViewHeader(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect,
    QSharedPointer<IThemeService> theme_service) :
  QWidget(parent), theme_service_(theme_service),
  layout_(layout), clip_(clip), effect_(effect),
  down_arrow_(":/widget/effect_control/down-arrow.svg", 20, 20) {

}

void EffectViewHeader::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  p.drawText(rect(), Qt::AlignVCenter | Qt::AlignLeft,
    QString::fromStdString(effect_->type()));
  p.drawImage(QPoint(0, 0), down_arrow_.image());

}

EffectView::EffectView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    QSharedPointer<IThemeService> theme_service) :
  QWidget(parent), theme_service_(theme_service),
  layout_(layout), clip_(clip), sequence_scroll_view_(sequence_scroll_view) {
  header_ = new EffectViewHeader(this, layout, sequence, clip, effect, theme_service);
  header_->show();
}

void EffectView::insertPropertyView(QWidget* view, int index) {
  Q_ASSERT(property_views_.size() >= index && index >= 0);
  property_views_.insert(property_views_.begin() + index, view);
  view->show();
  updateGeometry();
}

void EffectView::doLayout() {
  int y = 0;
  header_->setGeometry(0, 0, width(), 20);
  y += 20;
  for (auto property_view : property_views_) {
    property_view->move(0, y);
    QSize size_hint = property_view->sizeHint();
    property_view->resize(width(), size_hint.height());
    y += property_view->sizeHint().height();
  }
}

void EffectView::doPaint() {
  QPainter p(this);
}

bool EffectView::event(QEvent* event) {
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

QSize EffectView::sizeHint() const {
  int height = 0;
  for (auto property_view : property_views_)
    height += property_view->sizeHint().height();
  return QSize(width(), height);
}

}

}