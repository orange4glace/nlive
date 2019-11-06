#include "browser/widgets/monitor/sequence_renderer_view.h"

#include <QOpenGLContext>
#include <QPainter>
#include <QRandomGenerator>

namespace nlive {

namespace monitor_widget {

SequenceRendererView::SequenceRendererView(QWidget* widget, QSharedPointer<Sequence> sequence) :
  QOpenGLWidget(widget),
  sequence_(sequence), sequence_renderer_(nullptr) {
}

SequenceRendererView::~SequenceRendererView() {
  // TODO
  sequence_renderer_ = nullptr;
  QOpenGLWidget::~QOpenGLWidget();
}

void SequenceRendererView::scheduleRender() {
  update();
}

void SequenceRendererView::initializeGL() {
  initializeOpenGLFunctions();
  sequence_renderer_ =
    new video_renderer::SequenceRenderer(sequence_, context());
  connect(
    sequence_renderer_,
    &video_renderer::SequenceRenderer::onDidReadyData,
    this,
    &SequenceRendererView::scheduleRender);
  sequence_renderer_->initialize();
}

void SequenceRendererView::resizeGL(int w, int h) {
  glViewport(0,0,w,h);
  // doneCurrent();
}

void SequenceRendererView::paintGL() {
  sequence_renderer_->paint();
}

}

}