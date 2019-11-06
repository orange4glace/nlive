#ifndef NLIVE_MIRROR_SHADER_H_
#define NLIVE_MIRROR_SHADER_H_

#include "renderer/video_renderer/shader_program.h"

namespace nlive {

namespace video_renderer {

class MirrorShaderProgram : public ShaderProgram {
  
private:
  GLuint id_;
  GLuint position_;
  GLuint texCoord_;
  GLuint image_;
  GLuint position_buffer_;
  GLuint texCoord_buffer_;

public:
  inline MirrorShaderProgram(QOpenGLContext* gl) :
    ShaderProgram(gl, "MirrorShader") {
    compile(
        "#version 300 es\n"
        "in vec2 a_position;\n"
        "in vec2 a_texCoord;\n"
        "out vec2 v_texCoord;\n"
        "void main() {\n"
          "gl_Position = vec4(a_position, 0, 1);\n"
          "v_texCoord = a_texCoord;\n"
        "}",

        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_image;\n"
        "in vec2 v_texCoord;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
          "vec2 in_v_texCoord = vec2(1.0, 1.0) - v_texCoord;\n"
          "if (in_v_texCoord.x > 0.5)\n"
            "in_v_texCoord.x = -in_v_texCoord.x + 1.0;\n"
          "outColor = texture(u_image, in_v_texCoord).bgra;\n"
        "}");

    auto gf = gl->functions();
    position_ = gf->glGetAttribLocation(program_, "a_position");
    texCoord_ = gf->glGetAttribLocation(program_, "a_texCoord");
    image_ = gf->glGetAttribLocation(program_, "u_image");

    gf->glGenBuffers(1, &position_buffer_);
    gf->glGenBuffers(1, &texCoord_buffer_);
  }

  inline GLuint position() const { return position_; }
  inline GLuint texCoord() const { return texCoord_; }
  inline GLuint image() const { return image_; }
  inline GLuint position_buffer() const { return position_buffer_; }
  inline GLuint texCoord_buffer() const { return texCoord_buffer_; }

};

}

}

#endif