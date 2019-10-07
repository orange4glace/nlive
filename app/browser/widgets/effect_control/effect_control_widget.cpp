#include "browser/widgets/effect_control/effect_control_widget.h"

#include <QDebug>
#include <QPainter>

#include "model/sequence/sequence.h"
#include "model/effect/transform_effect.h"
#include "browser/widgets/timeline/timeline_widget_service.h"
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
  target_timeline_widget_(nullptr),
  sequence_view_(nullptr) {
  connect(timeline_widget_service_.get(), &ITimelineWidgetService::onDidChangeCurrentWidget, this, [this](timelinewidget::TimelineWidget* widget) {
    this->setTargetTimelineWidget(widget);
  });
  qDebug() << "Vi " << isVisible() << "\n";
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
  disconnect(timeline_widget_connection_);
  timeline_widget_connection_ = connect(timeline_widget, &timelinewidget::TimelineWidget::onDidChangeSequence,
    this, [this, timeline_widget](QSharedPointer<Sequence> sequence) {
    this->setTargetTimelineWidgetSequenceView(timeline_widget->sequence_view());
  });
}

void EffectControlWidget::setTargetTimelineWidgetSequenceView(timelinewidget::SequenceView* sequence_view) {
  qDebug() << "[EffectControlWidget] setTargetTimelineWidget " << sequence_view << "\n";
  if (sequence_view_ != nullptr) {
    delete sequence_view_;
  }
  if (sequence_view == nullptr) return;
  sequence_view_ = new SequenceView(this, layout_params_, sequence_view, theme_service_);
  sequence_view_->setGeometry(geometry());
  sequence_view_->show();
}

void EffectControlWidget::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  p.fillRect(rect(), Qt::green);
}

void EffectControlWidget::resizeEvent(QResizeEvent* event) {
  QDockWidget::resizeEvent(event);
  if (sequence_view_ != nullptr)
    sequence_view_->setGeometry(geometry());
}

}

}