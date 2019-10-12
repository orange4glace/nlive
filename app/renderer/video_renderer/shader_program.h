#ifndef NLIVE_VIDEO_RENDERER_SHADER_PROGRAM_H_
#define NLIVE_VIDEO_RENDERER_SHADER_PROGRAM_H_

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace nlive {

namespace video_renderer {

class ShaderProgram {
  
private:

protected:
  QOpenGLContext* gl_;
  GLuint program_;
  GLuint vert_shader_;
  GLuint frag_shader_;

  inline ShaderProgram(QOpenGLContext* gl) :
    gl_(gl) {
    
  }

  inline ~ShaderProgram() {
    auto gf = gl_->functions();
    gf->glDeleteShader(vert_shader_);
    gf->glDeleteShader(frag_shader_);
    gf->glDeleteProgram(program_);
  }

  inline void compile(
    const char* vert_shader_source,
    const char* frag_shader_source) {
    auto gf = gl_->functions();
    vert_shader_ = gf->glCreateShader(GL_VERTEX_SHADER);
    frag_shader_ = gf->glCreateShader(GL_FRAGMENT_SHADER);
    gf->glShaderSource(vert_shader_, 1, &vert_shader_source, NULL);
    gf->glShaderSource(frag_shader_, 1, &frag_shader_source, NULL);

    gf->glCompileShader(vert_shader_);
    gf->glCompileShader(frag_shader_);

    program_ = gf->glCreateProgram();
    gf->glAttachShader(program_, vert_shader_);
    gf->glAttachShader(program_, frag_shader_);
    gf->glLinkProgram(program_);
  } 

public:
  inline GLuint id() const {
    return program_;
  }

};

}

}

#endif