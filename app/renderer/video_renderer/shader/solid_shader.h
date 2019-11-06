#ifndef NLIVE_VIDEO_RENDERER_SOLID_SHADER_H_
#define NLIVE_VIDEO_RENDERER_SOLID_SHADER_H_

#include "renderer/video_renderer/shader_program.h"

namespace nlive {

namespace video_renderer {

class SolidShaderProgram : public ShaderProgram {
  
private:
  GLuint id_;
  GLuint position_;
  GLuint position_buffer_;

public:
  inline SolidShaderProgram(QOpenGLContext* gl) :
    ShaderProgram(gl, "SolidShader") {
    compile(
        "#version 300 es\n"
        "in vec2 a_position;\n"
        "void main() {\n"
          "gl_Position = vec4(a_position, 0, 1);\n"
        "}",

        "#version 300 es\n"
        "precision mediump float;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
          "outColor = vec4(0.5, 0.7, 0.0, 1.0);\n"
        "}");

    auto gf = gl->functions();
    position_ = gf->glGetAttribLocation(program_, "a_position");

    gf->glGenBuffers(1, &position_buffer_);
  }

  inline GLuint position() const { return position_; }
  inline GLuint position_buffer() const { return position_buffer_; }

};

}

}

#endif