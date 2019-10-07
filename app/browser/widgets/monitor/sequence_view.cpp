#include "browser/widgets/monitor/sequence_view.h"

#include <QOpenGLContext>
#include <QPainter>
#include <QRandomGenerator>

namespace nlive {

namespace monitor_widget {

namespace {
  GLuint vert_shader_;
  GLuint frag_shader_;
  GLuint program_;
  GLuint position_loc_;
  GLuint texCoord_loc_;
  GLuint image_loc_;
  GLuint position_buffer_;
  GLuint texCoord_buffer_;
  GLuint tex_;
}

SequenceView::SequenceView(QWidget* widget, QSharedPointer<Sequence> sequence) :
  QOpenGLWidget(widget),
  sequence_(sequence), sequence_renderer_(nullptr) {
  show();
}

void SequenceView::scheduleRender() {
  qDebug() << "Schedule Render\n";
  update();
}

void SequenceView::initializeGL() {
  initializeOpenGLFunctions();
  sequence_renderer_ =
    new video_renderer::SequenceRenderer(sequence_, context());
  connect(
    sequence_renderer_,
    &video_renderer::SequenceRenderer::onDidReadyData,
    this,
    &SequenceView::scheduleRender);
  sequence_renderer_->initialize();
}

void SequenceView::resizeGL(int w, int h) {
  qDebug() << "RESIZE GL " << w << " " <<  h << "\n";
  glViewport(0,0,w,h);
  // doneCurrent();
}

namespace {
  int z = 0;
}

void SequenceView::paintGL() {
  qDebug() << "PAINT GL\n";
  sequence_renderer_->paint();
}

}

}