#include "browser/widgets/timeline/scrollview/sequencescrollview.h"

#include "model/sequence/sequence.h"
#include "platform/theme/themeservice.h"

#include <QtMath>
#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace nlive {

/*#region Indicator*/
SequenceScrollView::Indicator::Indicator(QWidget* parent, IThemeService* theme_service) :
  QWidget(parent), theme_service_(theme_service) {
  setAttribute(Qt::WA_TransparentForMouseEvents);
}

void SequenceScrollView::Indicator::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  QPainter p(this);
  p.setPen(QPen(theme.primaryColor()));
  p.drawLine(0, 0, 0, height());
}
/*#endregion*/

SequenceScrollView::SequenceScrollView(
  QWidget* const parent,
  QWidget* const content_widget,
  QSharedPointer<Sequence> sequence,
  IThemeService* theme_service) :
  QWidget(parent),
  theme_service_(theme_service),
  sequence_(sequence),
  content_widget_(nullptr),
  scrollbar_(this, 0.0f, 1.0f, theme_service),
  indicator_(this, theme_service) {
  setContentWidget(content_widget);
  doUpdate();

  QObject::connect(&scrollbar_, &SequenceScrollViewScrollbar::onDidUpdate, this, [this]() {
    doUpdate();
  });
  QObject::connect(sequence_.get(), &Sequence::onDidChangeDuration, this, [this]() {
    doUpdate();
  });
  QObject::connect(sequence_.get(), &Sequence::onDidChangeCurrentTime, this, [this]() {
    doUpdate();
  });
}

void SequenceScrollView::setContentWidget(QWidget* widget) {
  if (widget != nullptr) 
  widget->setParent(this);
  content_widget_ = widget;
  indicator_.raise();
  doResize();
}

void SequenceScrollView::update(qreal scroll_start, qreal scroll_end) {
  scrollbar_.setValue(scroll_start, scroll_end);
}

// Reszie ScrollView
void SequenceScrollView::doResize() {
  int ruler_height = 30;
  int scrollbar_height = 15;

  // Indicator
  indicator_.resize(1, height() - ruler_height - scrollbar_height);

  // Calculate scrollbar area
  int scrollbar_y = height() - scrollbar_height;
  scrollbar_.move(0, scrollbar_y);
  scrollbar_.resize(width(), scrollbar_height);

  // Calculate content area
  if (content_widget_ != nullptr) {
    int content_x = 0;
    int content_y = ruler_height;
    int content_w = width();
    int content_h = height() - ruler_height - scrollbar_height;
    content_widget_->move(content_x, content_y);
    content_widget_->resize(content_w, content_h);
  }
}

// Update ScrollView
void SequenceScrollView::doUpdate() {
  int start_time = qFloor(sequence_->duration() * scrollbar_.start());
  int end_time = qCeil(sequence_->duration() * scrollbar_.end());
  // Initial value
  unit_frame_time_ = 30;
  unit_width_ = width() / ((end_time - start_time) / unit_frame_time_);
  if (unit_width_ <= 0) return;
  int MULTIPLIER[] = { 5, 2, 3, 2 };
  int multiplier_i = 0;
  if (unit_width_ > 150) {
    while (true) {
      qreal cand = unit_width_ / MULTIPLIER[multiplier_i];
      if (cand < 150) break;
      unit_width_ = cand;
      unit_frame_time_ /= MULTIPLIER[multiplier_i];
      multiplier_i = (++multiplier_i % 4/* Length of MULTIPLIER */);
    }
  }
  else {
    while (unit_width_ < 150) {
      unit_width_ = unit_width_ * MULTIPLIER[multiplier_i];
      unit_frame_time_ *= MULTIPLIER[multiplier_i];
      multiplier_i = (++multiplier_i % 4/* Length of MULTIPLIER */);
    }
  }
  px_per_frame_ = unit_width_ / unit_frame_time_;

  start_time_ = start_time;
  end_time_ = end_time;

  // Update UI
  int ruler_height = 30;

  // Indicator
  int indicator_pos = getPositionRelativeToView(sequence_->current_time());
  indicator_.move(indicator_pos, ruler_height);

  QWidget::update();
  
  emit onDidUpdate();
}

void SequenceScrollView::mouseMoveEvent(QMouseEvent* event) {
  int64_t time = getTimeRelativeToView(event->pos().x());
  sequence_->setCurrentTime(time);
}

void SequenceScrollView::paintEvent(QPaintEvent* event) {
  auto& theme = theme_service_->getTheme();
  int ruler_height = 30;

  QPainter p(this);

  QFont f;
  f.setPixelSize(11);
  p.setFont(f);
  p.setPen(theme.surfaceTextColor());

  // Draw ruler
  auto format = [](int millisecond) -> QString {
    auto dd = [](int num) -> QString {
      QString s = QString::number(num);
      if (s.size() == 0) return "00";
      else if (s.size() == 1) return "0" + s;
      else if (s.size() > 2) return s.mid(0, 2);
      return s;
    };
    int milli = millisecond % 30;
    millisecond = millisecond / 30;
    int sec = millisecond % 60;
    millisecond = millisecond / 60;
    int min = millisecond % 60;
    millisecond = millisecond / 60;
    int hour = millisecond;
    return dd(hour) + ":" + dd(min) + ":" + dd(sec) + ":" + dd(milli);
  };
  int start_count = qFloor(start_time_ / unit_frame_time_);
  int end_count = qCeil(end_time_ / unit_frame_time_);
  qreal value = start_count * unit_frame_time_;
  qreal translate_x = (start_time_ - value) * px_per_frame_;
  p.save();
  p.translate(-translate_x, 0);
  for (int i = 0; i < end_count - start_count; i ++) {
    p.save();
    p.translate(-30, 0);
    p.drawText(QPoint(0, 10), format(value));
    p.restore();
    p.drawLine(0, 15, 0, 29);
    for (int j = 0; j < 9; j ++) {
      p.translate(unit_width_ / 10, 0);
      p.drawLine(0, 22.5, 0, 29);
    }
    p.translate(unit_width_ / 10, 0);
    value += unit_frame_time_;
  }
  p.restore();

  // Draw indicator tip
  p.setRenderHint(QPainter::Antialiasing);
  int indicator_pos = getPositionRelativeToView(sequence_->current_time());
  QPainterPath path;
  path.moveTo(indicator_pos, ruler_height);
  path.lineTo(indicator_pos - 9, ruler_height - 13);
  path.lineTo(indicator_pos + 9, ruler_height - 13);
  p.fillPath(path, theme.primaryColor());
}

void SequenceScrollView::resizeEvent(QResizeEvent* event) {
  doResize();
  QWidget::resizeEvent(event);
}

int SequenceScrollView::getTimeRelativeToView(int pixel) const {
  return roundf(start_time_ + pixel / px_per_frame_);
}

int SequenceScrollView::getTimeAmountRelativeToView(int pixel) const {
  return roundf(pixel / px_per_frame_);
}

int SequenceScrollView::getPositionRelativeToView(int time) const {
  return floorf((time - start_time_) * px_per_frame_);
}

int SequenceScrollView::getPixelAmountRelativeToView(int time) const {
  return roundf(time * px_per_frame_);
}

}