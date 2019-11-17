#include "browser/widgets/effect_control/sequence_view.h"

#include <QDebug>
#include <QPainter>

#include "model/sequence/sequence.h"
#include "model/sequence/clip.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "browser/widgets/timeline/clipview.h"
#include "browser/widgets/effect_control/clip_view.h"

namespace nlive {

namespace effect_control {

SequenceView::SequenceView(
  QWidget* parent,
  QSharedPointer<EffectControlLayout> layout,
  timeline_widget::SequenceView* timeline_widget_sequence_view,
  QSharedPointer<IThemeService> theme_service,
  QSharedPointer<IMementoService> memento_service) :
  QWidget(parent), theme_service_(theme_service), memento_service_(memento_service),
  layout_params_(layout),
  timeline_widget_sequence_view_(timeline_widget_sequence_view),
  clip_(nullptr), clip_view_(nullptr) {
  sequence_scroll_view_ = new SequenceScrollView(this, nullptr, timeline_widget_sequence_view->sequence(), theme_service_);
  sequence_scroll_view_->show();

  auto& theme = theme_service_->getTheme();
  // form_background_ = new SolidBox(this, theme.surfaceDarkColor());
  // timeline_background_ = new SolidBox(this, theme.surfaceColor());
  // form_background_->lower();
  // timeline_background_->lower();
  // theme_service_->onDidUpdate.connect(SIG2_TRACK(sig2_t<void()>::slot_type(
  //   [this]() {
  //     auto& theme = theme_service_->getTheme();
  //     form_background_->setColor(theme.surfaceDarkColor());
  //     timeline_background_->setColor(theme.surfaceColor());
  //   }
  // )));

  this->handleDidChangeFocusedClips();
  timeline_widget_sequence_view_->timeline_view()->onDidChangeFocusedClips.connect(
    sig2_t<void ()>::slot_type([this]() {
      this->handleDidChangeFocusedClips();
    }).track(__sig_scope_));
  show();
}

void SequenceView::handleDidChangeFocusedClips() {
  auto& focused_clip_views = timeline_widget_sequence_view_->timeline_view()->focused_clip_views();
  if (focused_clip_views.size() != 1) return;
  auto clip = (*focused_clip_views.begin())->clip();
  createClipView(clip);
}

void SequenceView::createClipView(QSharedPointer<Clip> clip) {
  clearClipView();
  clip_ = clip;
  qDebug() << "[SequenceView] createClipView " << clip << ", # of effects = " << clip->effects().size() << "\n";
  auto sequence = timeline_widget_sequence_view_->sequence();
  clip_view_ = new ClipView(this, layout_params_, sequence, clip, sequence_scroll_view_,
    theme_service_, memento_service_);
  clip_view_->setGeometry(0, 30, width(), clip_view_->sizeHint().height());
  clip_view_->show();
  clip_view_->raise();
}

void SequenceView::clearClipView() {
  if (clip_view_ == nullptr) return;
  delete clip_view_;
}

void SequenceView::paintEvent(QPaintEvent* event) {
  QPainter p(this);
  auto& theme = theme_service_->getTheme();
  p.setPen(theme.surfaceBrightColor());
  p.drawLine(layout_params_->form_width() - 1, 0,
    layout_params_->form_width() - 1, height());
}

void SequenceView::resizeEvent(QResizeEvent* event) {
  if (clip_view_ != nullptr) {
    clip_view_->setGeometry(0, 30, width(), clip_view_->sizeHint().height());
  }
  if (sequence_scroll_view_ != nullptr) {
    sequence_scroll_view_->setGeometry(
        layout_params_->side_timeline_ratio() * width(), 0,
        (1 - layout_params_->side_timeline_ratio()) * width(), height());
    int form_width = width() - layout_params_->side_timeline_ratio() * width();
    int form_value_width = form_width * 0.5;
    layout_params_->setWidth(width());
    layout_params_->setFormValueWidth(form_value_width);
    // form_background_->setGeometry(0, 0, form_width, height());
    // timeline_background_->setGeometry(form_width, 0, width() - form_width, height());
  }
}

}

}