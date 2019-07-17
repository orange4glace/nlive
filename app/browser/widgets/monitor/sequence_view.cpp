#include "browser/widgets/monitor/sequence_view.h"

#include <QOpenGLContext>
#include <QPainter>

namespace nlive {

namespace monitor_widget {

SequenceView::SequenceView(QWidget* widget, QSharedPointer<Sequence> sequence) :
  QOpenGLWidget(widget),
  sequence_(sequence) {
  // format().setVersion(3, 2);
  show();
}

void SequenceView::scheduleRender() {
  update();
}

void SequenceView::initializeGL() {
  initializeOpenGLFunctions();
  char* log = new char[1000];
  // auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* vert_shader_source =
    "#version 300 es\n"
    "in vec4 a_position;\n"
    "void main() {\n"
    " gl_Position = a_position;\n"
    "}";
  program.addShaderFromSourceCode(QOpenGLShader::Vertex, vert_shader_source);
  // qDebug() << "vert shader id = " << vert_shader << " " << vert_shader_source <<  "\n";
  GLint a;
  // glShaderSource(vert_shader, 5, vert_shader_source, &a);
  // glCompileShader(vert_shader);
  // glGetShaderInfoLog(vert_shader, 1000, nullptr, log);
  // qDebug() << log << "\n";


  // auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  // const char* frag_shader_source[] = {
  //   "#version 300 es",
  //   "precision mediump float;",
  //   "out vec4 outColor;",
  //   "void main() {",
  //   " outColor = vec4(1, 0, 0.5, 1);",
  //   "}"
  // };
  // glShaderSource(frag_shader, 6, frag_shader_source, nullptr);
  // glCompileShader(frag_shader);
  // glGetShaderInfoLog(vert_shader, 1000, nullptr, log);
  // qDebug() << log << "\n";

  // doneCurrent();
  // sequence_renderer_ =
  //   new video_renderer::SequenceRenderer(sequence_, context());
  // connect(
  //   sequence_renderer_,
  //   &video_renderer::SequenceRenderer::onDidReadyData,
  //   this,
  //   &SequenceView::scheduleRender);
}

void SequenceView::resizeGL(int w, int h) {
  doneCurrent();
}

void SequenceView::paintGL() {
  qDebug() << "PaintGL\n";
  doneCurrent();
}

}

}