#include "browser/widgets/monitor/sequence_renderer_view.h"

#include <QOpenGLContext>
#include <QPainter>
#include <QRandomGenerator>

namespace nlive {

namespace monitor_widget {

SequenceRendererView::SequenceRendererView(QWidget* widget, timeline_widget::SequenceView* timeline_widget_sequence_view,
  sptr<PlayService> play_service) :
  QOpenGLWidget(widget), play_service_(play_service),
  timeline_widget_sequence_view_(timeline_widget_sequence_view),
  sequence_(timeline_widget_sequence_view->sequence()), renderer_(nullptr) {
}

SequenceRendererView::~SequenceRendererView() {
  // TODO
  if (renderer_) {
    delete renderer_;
  }
}

void SequenceRendererView::scheduleRender() {
  update();
}

void SequenceRendererView::initializeGL() {
  initializeOpenGLFunctions();
  qDebug() << context()->format();
  renderer_ = new video_renderer::SequenceRenderer(sequence_, context());
  connect(
    renderer_,
    &video_renderer::SequenceRenderer::onDidReadyData,
    this,
    &SequenceRendererView::scheduleRender);
  renderer_->initialize();
  timeline_widget_sequence_view_->sequence_playable()->setVideoRenderer(renderer_);
}

void SequenceRendererView::resizeGL(int w, int h) {
  glViewport(0,0,w,h);
  // doneCurrent();
}

void SequenceRendererView::paintGL() {
  renderer_->paint();
}

}

}