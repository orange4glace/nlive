#include "browser/widgets/effect_control/effect_view.h"

#include <QEvent>
#include <QPainter>
#include "base/common/perf.h"
#include "base/layout/fillparentbox.h"
#include "base/ui/text_box.h"
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
  FlexLayout(parent), theme_service_(theme_service),
  layout_(layout), clip_(clip), effect_(effect) {
  auto& theme = theme_service_->getTheme();
  arrow_button_ = new SvgButton(this, ":/widget/effect_control/down-arrow.svg");
  FillParentBox* button_container = new FillParentBox(this, arrow_button_);
  button_container->setFlexBasis(20)->setFlexShrink(0)->setFlexGrow(0);
  TextBox* label_view = new TextBox(this, QString::fromStdString(effect->type()), Qt::AlignVCenter);
  label_view->setColor(theme.surfaceTextColor())->setPadding(Div::LEFT, 10);
  this->addChild(button_container);
  this->addChild(label_view);
  connect(arrow_button_, &QPushButton::clicked,
    [this]() {
      onArrowButtonClicked(opened_);
    });
}

void EffectViewHeader::setOpened(bool value) {
  opened_ = value;
  update();
}

void EffectViewHeader::paintEvent(QPaintEvent* e) {
  Div::paintEvent(e);
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  int form_width = width() - layout_->timeline_width();
  p.setPen(theme.surfaceColor());
  p.drawLine(0, height() - 1, form_width, height() - 1);
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
      QSize size_hint = property_view->sizeHint();
      property_view->setGeometry(20, y, width() - 20, size_hint.height());
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