#ifndef NLIVE_GRAY_SCALE_SHADER_H_
#define NLIVE_GRAY_SCALE_SHADER_H_

#include "renderer/video_renderer/shader_program.h"

namespace nlive {

namespace video_renderer {

class GrayScaleShaderProgram : public ShaderProgram {
  
private:
  GLuint id_;
  GLuint position_;
  GLuint texCoord_;
  GLuint value_;
  GLuint image_;
  GLuint position_buffer_;
  GLuint texCoord_buffer_;

public:
  inline GrayScaleShaderProgram(QOpenGLContext* gl) :
    ShaderProgram(gl, "GrayScaleShader") {
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
        "uniform float u_value;\n"
        "in vec2 v_texCoord;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
          "outColor = texture(u_image, v_texCoord).rgba;\n"
          "const vec3 W = vec3(0.2125, 0.7154, 0.0721);\n"
          "vec3 intensity = vec3(dot(outColor.rgb, W));\n"
          "outColor.rgb = mix(intensity, outColor.rgb, u_value);\n"
        "}");

    auto gf = gl->functions();
    position_ = gf->glGetAttribLocation(program_, "a_position");
    texCoord_ = gf->glGetAttribLocation(program_, "a_texCoord");
    value_ = gf->glGetUniformLocation(program_, "u_value");
    image_ = gf->glGetAttribLocation(program_, "u_image");

    gf->glGenBuffers(1, &position_buffer_);
    gf->glGenBuffers(1, &texCoord_buffer_);
  }

  inline GLuint position() const { return position_; }
  inline GLuint texCoord() const { return texCoord_; }
  inline GLuint value() const { return value_; }
  inline GLuint image() const { return image_; }
  inline GLuint position_buffer() const { return position_buffer_; }
  inline GLuint texCoord_buffer() const { return texCoord_buffer_; }

};

}

}

#endif