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
  layout_(layout), clip_(clip), effect_(effect) {
  arrow_button_ = new SvgButton(this, ":/widget/effect_control/down-arrow.svg");
  arrow_button_->setGeometry(0, 0, 20, 20);
  connect(arrow_button_, &QPushButton::clicked,
    [this]() {
      onArrowButtonClicked(opened_);
    });
}

void EffectViewHeader::paintEvent(QPaintEvent* e) {
  QPainter p(this);
  QRect r = rect(); r.setLeft(20);
  p.drawText(r, Qt::AlignVCenter | Qt::AlignLeft,
    QString::fromStdString(effect_->type()));

}

void EffectViewHeader::setOpened(bool value) {
  opened_ = value;
  update();
}

QSize EffectViewHeader::sizeHint() const {
  return QSize(-1, 20);
}

EffectView::EffectView(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IMementoService> memento_service) :
  QWidget(parent), theme_service_(theme_service), memento_service_(memento_service),
  opened_(true), layout_(layout), clip_(clip), effect_(effect), 
  sequence_scroll_view_(sequence_scroll_view) {
  header_ = new EffectViewHeader(this, layout, sequence, clip, effect, theme_service);
  header_->show();

  auto scope = memento_service_->getScope({effect_->id(), "HE"});
  if (!scope->hasValue()) scope->setValue(true);
  setOpened(scope->getValue<bool>());


  header_->onArrowButtonClicked.connect(SIG2_TRACK(sig2_t<void (bool)>::slot_type(
    [this](bool value) {
      setOpened(!value);
    })));
}

void EffectView::insertPropertyView(QWidget* view, int index) {
  Q_ASSERT(property_views_.size() >= index && index >= 0);
  property_views_.insert(property_views_.begin() + index, view);
  view->show();
  updateGeometry();
  doLayout();
}

void EffectView::doLayout() {
  int y = 0;
  header_->setGeometry(0, 0, width(), header_->sizeHint().height());
  y += header_->sizeHint().height();
  if (opened_) {
    for (auto property_view : property_views_) {
      property_view->show();
      property_view->move(0, y);
      QSize size_hint = property_view->sizeHint();
      property_view->resize(width(), size_hint.height());
      y += property_view->sizeHint().height();
    }
  }
  else {
    for (auto property_view : property_views_) {
      property_view->hide();
    }
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

namespace {
  int z= 0;
}

void EffectView::setOpened(bool value) {
  auto scope = memento_service_->getScope({effect_->id(), "HE"});
  scope->setValue(value);
  opened_ = value;
  header_->setOpened(value);
  doLayout();
  updateGeometry();
}

QSize EffectView::sizeHint() const {
  int height = header_->sizeHint().height();
  if (opened_) {
    for (auto property_view : property_views_)
      height += property_view->sizeHint().height();
  }
  return QSize(width(), height);
}

}

}