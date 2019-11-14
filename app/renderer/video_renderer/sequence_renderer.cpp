#include "renderer/video_renderer/sequence_renderer.h"

#include <QDebug>
#include <QOpenGLFunctions>
#include "base/common/perf.h"

namespace nlive {

namespace video_renderer {

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

SequenceRenderer::SequenceRenderer(
  QSharedPointer<Sequence> sequence,
  QOpenGLContext* target_gl) :
  sequence_(sequence),
  target_gl_(target_gl) {
  renderer_ = QSharedPointer<Renderer>(new Renderer(target_gl, sequence->width(), sequence->height()));
  
  sequence->onDirty.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
    boost::bind(&SequenceRenderer::render, this))));
  connect(renderer_.get(), &Renderer::onDidReadyData, this, [this]() {
    emit onDidReadyData();
  });

  renderer_->start();
}

SequenceRenderer::~SequenceRenderer() {
  renderer_->wait();
}

// std::unique_ptr<RenderTexture> SequenceRenderer::getRenderData() {
//   return std::move(renderer_->getRenderData());
// }

void SequenceRenderer::initialize() {
  auto gf = target_gl_->functions();
  gf->initializeOpenGLFunctions();
  gf->glClearColor(0,1,0,1);
  vert_shader_ = gf->glCreateShader(GL_VERTEX_SHADER);
  frag_shader_ = gf->glCreateShader(GL_FRAGMENT_SHADER);

  const char* vert_shader_source_ =
    "#version 300 es\n"
    "in vec2 a_position;\n"
    "in vec2 a_texCoord;\n"
    "out vec2 v_texCoord;\n"
    "void main() {\n"
      "gl_Position = vec4(a_position, 0, 1);\n"
      "v_texCoord = a_texCoord;\n"
    "}";
  const char* frag_shader_source_ = 
    "#version 300 es\n"
    "precision mediump float;\n"
    "uniform sampler2D u_image;\n"
    "in vec2 v_texCoord;\n"
    "out vec4 outColor;\n"
    "void main() {\n"
      "outColor = texture(u_image, v_texCoord);\n"
    "}";
  gf->glShaderSource(vert_shader_, 1, &vert_shader_source_, NULL);
  gf->glShaderSource(frag_shader_, 1, &frag_shader_source_, NULL);

  gf->glCompileShader(vert_shader_);
  gf->glCompileShader(frag_shader_);

  program_ = gf->glCreateProgram();
  gf->glAttachShader(program_, vert_shader_);
  gf->glAttachShader(program_, frag_shader_);
  gf->glLinkProgram(program_);

  position_loc_ = gf->glGetAttribLocation(program_, "a_position");
  texCoord_loc_ = gf->glGetAttribLocation(program_, "a_texCoord");
  image_loc_ = gf->glGetAttribLocation(program_, "u_image");

  gf->glGenBuffers(1, &position_buffer_);
  gf->glGenBuffers(1, &texCoord_buffer_);

  gf->glGenTextures(1, &tex_);
}

void SequenceRenderer::render() {
  auto command_buffer = sequence_->renderVideo(sequence_->current_time());
  renderer_->render(command_buffer);
}

void SequenceRenderer::paint() {
  renderer_->buffer_swap_mutex()->lock();
  if (!renderer_->context()->initialized()) {
    // No need to lock to check if intialized since
    // it is guaranteed that there's no chance to write
    // `initialized` variable at this moment
    renderer_->buffer_swap_mutex()->unlock();
    return;
  }
  auto gf = target_gl_->functions();
  auto front_render_texture = renderer_->context()->getFrontRenderTexture();
// std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  gf->glClearColor(0, 0, 0, 1.0f);
  gf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gf->glUseProgram(program_);
  gf->glActiveTexture(GL_TEXTURE0);
  gf->glBindTexture(GL_TEXTURE_2D, front_render_texture.texture);
  gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  gf->glUniform1i(image_loc_, 0);
  gf->glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
  float position[12] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f
  };
  gf->glBufferData(GL_ARRAY_BUFFER, 4 * 12, (void*)position, GL_DYNAMIC_DRAW);
  gf->glEnableVertexAttribArray(position_loc_);
  gf->glVertexAttribPointer(position_loc_, 2, GL_FLOAT, false, 0, (void*)0);
  gf->glBindBuffer(GL_ARRAY_BUFFER, texCoord_buffer_);
  float texCoord[12] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
  };
  gf->glBufferData(GL_ARRAY_BUFFER, 4 * 12, (void*)texCoord, GL_DYNAMIC_DRAW);
  gf->glEnableVertexAttribArray(texCoord_loc_);
  gf->glVertexAttribPointer(texCoord_loc_, 2, GL_FLOAT, false, 0, (void*)0);
  gf->glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
  gf->glDrawArrays(GL_TRIANGLES, 0, 6);
  renderer_->buffer_swap_mutex()->unlock();
// std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
// qDebug() << "Measured " << "Sequence Render" << " perf time = " << " " << sec.count();
}

}

}