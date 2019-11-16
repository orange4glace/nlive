#include "browser/widgets/monitor/sequence_renderer_view.h"

#include <QOpenGLContext>
#include <QPainter>
#include <QRandomGenerator>

namespace nlive {

namespace monitor_widget {

SequenceRendererView::SequenceRendererView(QWidget* widget, QSharedPointer<Sequence> sequence,
  QSharedPointer<PlayService> play_service) :
  QOpenGLWidget(widget), play_service_(play_service),
  sequence_(sequence), player_(nullptr) {
  player_ = new SequencePlayable(this, sequence);
  play_service_->play(player_);
}

SequenceRendererView::~SequenceRendererView() {
  // TODO
  delete player_;
}

void SequenceRendererView::scheduleRender() {
  update();
}

void SequenceRendererView::initializeGL() {
  initializeOpenGLFunctions();
  player_->initializeVideoRenderer(context());
  connect(
    player_->sequence_video_renderer(),
    &video_renderer::SequenceRenderer::onDidReadyData,
    this,
    &SequenceRendererView::scheduleRender);
}

void SequenceRendererView::resizeGL(int w, int h) {
  glViewport(0,0,w,h);
  // doneCurrent();
}

void SequenceRendererView::paintGL() {
  player_->sequence_video_renderer()->paint();
}

}

}