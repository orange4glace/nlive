#ifndef NLIVE_SIMPLE_RENDER_COMMAND_H_
#define NLIVE_SIMPLE_RENDER_COMMAND_H_

#include <QSharedPointer>
#include <QOpenGLFunctions>
#include <stdint.h>

#include "renderer/video_renderer/render_command.h"

namespace nlive {

namespace video_renderer {

class SimpleRenderCommand : public RenderCommand {

private:
  uint8_t* buffer_;
  bool is_external_buffer_;

  bool initialized_;
  GLuint vert_shader_;
  GLuint frag_shader_;
  GLuint program_;
  GLuint position_loc_;
  GLuint position_buffer_;

  float r, g, b, x, y;

  inline void initialize(QOpenGLContext* gl) {
    if (initialized_) return;
    initialized_ = true;
    auto glf = gl->functions();
    vert_shader_ = glf->glCreateShader(GL_VERTEX_SHADER);
    frag_shader_ = glf->glCreateShader(GL_FRAGMENT_SHADER);
    
    const char* vert_shader_source_ =
        "#version 300 es\n"
        "in vec2 a_position;\n"
        "void main() {\n"
          "gl_Position = vec4(a_position, 0, 1);\n"
        "}";
    const char* frag_shader_source_ = 
        "#version 300 es\n"
        "precision mediump float;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
          "outColor = vec4(1, 0, 0, 1);\n"
        "}";
    glf->glShaderSource(vert_shader_, 1, &vert_shader_source_, NULL);
    glf->glShaderSource(frag_shader_, 1, &frag_shader_source_, NULL);

    glf->glCompileShader(vert_shader_);
    glf->glCompileShader(frag_shader_);

    program_ = glf->glCreateProgram();
    glf->glAttachShader(program_, vert_shader_);
    glf->glAttachShader(program_, frag_shader_);
    glf->glLinkProgram(program_);

    position_loc_ = glf->glGetAttribLocation(program_, "a_position");

    glf->glGenBuffers(1, &position_buffer_);
  }

public:
  inline SimpleRenderCommand(float r, float g, float b, float x, float y) :
    buffer_(nullptr), is_external_buffer_(false), initialized_(false),
    r(r), g(g), b(b), x(x), y(y) {
  }

  inline ~SimpleRenderCommand() {
    if (buffer_ && !is_external_buffer_) delete buffer_;
  }

  inline void render(QSharedPointer<RendererContext> ctx) {
    QOpenGLContext* gl = ctx->gl();
    initialize(gl);
    auto glf = gl->functions();
    RenderTexture rt = ctx->getBackRenderTexture();
    
    glf->glBindFramebuffer(GL_FRAMEBUFFER, rt.framebuffer);
    glf->glViewport(0, 0, 256, 256);
    glf->glUseProgram(program_);
    glf->glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
    glf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float position[12] = {
      x, y,
      1.0f, 0.0f,
      0.0f, 1.0f,
      0.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 1.0f
    };
    glf->glBufferData(GL_ARRAY_BUFFER, 4 * 12, (void*)position, GL_STATIC_DRAW);
    glf->glEnableVertexAttribArray(position_loc_);
    glf->glVertexAttribPointer(position_loc_, 2, GL_FLOAT, false, 0, (void*)0);
    glf->glDrawArrays(GL_TRIANGLES, 0, 6);
    glf->glDisableVertexAttribArray(position_loc_);
  }

};

}

}

#endif