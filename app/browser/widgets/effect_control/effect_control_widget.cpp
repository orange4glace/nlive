#include "browser/widgets/effect_control/effect_control_widget.h"

#include <QDebug>
#include <QPainter>

#include "model/sequence/sequence.h"
#include "model/effect/transform_effect.h"
#include "browser/widgets/timeline/timeline_widget_service.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "browser/widgets/effect_control/sequence_view.h"
#include "browser/widgets/effect_control/effect_view_factory.h"
#include "browser/widgets/effect_control/transform/transform_effect_view.h"

namespace nlive {

namespace effect_control {

void EffectControlWidget::Initialize() {
  // Register pre-defined views
  EffectViewFactoryRegistry::registerFactory(
    effect::TransformEffect::TYPE,
    new TransformEffectViewFactory());
}

EffectControlWidget::EffectControlWidget(
  QWidget* parent,
  QSharedPointer<IThemeService> theme_service,
  QSharedPointer<ITimelineWidgetService> timeline_widget_service) :
  QDockWidget(parent), 
  theme_service_(theme_service),
  timeline_widget_service_(timeline_widget_service),
  sequence_scroll_view_(nullptr),
  target_timeline_widget_(nullptr),
  sequence_view_(nullptr) {
  layout_params_ = QSharedPointer<EffectControlLayout>(new EffectControlLayout(0.5));
  timeline_widget_service_->onDidChangeCurrentWidget.connect(
    sig2_t<void (timelinewidget::TimelineWidget*)>::slot_type(
    [this](timelinewidget::TimelineWidget* widget) {
      this->setTargetTimelineWidget(widget);
    }).track(__sig_scope_));
}

void EffectControlWidget::setTargetTimelineWidget(timelinewidget::TimelineWidget* timeline_widget) {
  qDebug() << "[EffectControlWidget] setTargetTimelineWidget " << timeline_widget << "\n";
  if (target_timeline_widget_ != nullptr) {
    target_timeline_widget_ = nullptr;
  }
  if (timeline_widget == nullptr) {
    setTargetTimelineWidgetSequenceView(nullptr);
    return;
  }
  target_timeline_widget_ = timeline_widget;
  setTargetTimelineWidgetSequenceView(timeline_widget->sequence_view());
  timeline_widget_connection_.disconnect();
  timeline_widget_connection_ = timeline_widget->onDidChangeSequence.connect(
    sig2_t<void (QSharedPointer<Sequence> sequence)>::slot_type(
      [this, timeline_widget](QSharedPointer<Sequence> sequence) {
      this->setTargetTimelineWidgetSequenceView(timeline_widget->sequence_view());
    }));
}

void EffectControlWidget::setTargetTimelineWidgetSequenceView(timelinewidget::SequenceView* sequence_view) {
  qDebug() << "[EffectControlWidget] setTargetTimelineWidget " << sequence_view << "\n";
  if (sequence_view_ != nullptr)
    delete sequence_view_;
  if (sequence_scroll_view_ != nullptr)
    delete sequence_scroll_view_;
  if (sequence_view == nullptr) return;
  qDebug() << "[EffectControlWidget] Create a SequenceView\n";
  sequence_scroll_view_ = new SequenceScrollView(this, nullptr, sequence_view->sequence(), theme_service_);
  sequence_scroll_view_->show();
  sequence_view_ = new SequenceView(this, layout_params_, sequence_view, sequence_scroll_view_, theme_service_);
  sequence_view_->resize(size());
  sequence_view_->show();
  updateGeometry();
}

void EffectControlWidget::paintEvent(QPaintEvent* event) {
  QPainter p(this);
}

void EffectControlWidget::resizeEvent(QResizeEvent* event) {
  QDockWidget::resizeEvent(event);
  if (sequence_view_ != nullptr) {
    sequence_view_->setGeometry(0, 30, width(), height() - 15);
  }
  if (sequence_scroll_view_ != nullptr) {
    sequence_scroll_view_->setGeometry(
        layout_params_->side_timeline_ratio() * width(), 0,
        (1 - layout_params_->side_timeline_ratio()) * width(), height());
  }
}

}

}