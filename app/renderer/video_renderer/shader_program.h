#ifndef NLIVE_VIDEO_RENDERER_SHADER_PROGRAM_H_
#define NLIVE_VIDEO_RENDERER_SHADER_PROGRAM_H_

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <string>
#include "platform/logger/logger.h"

namespace nlive {

namespace video_renderer {

class ShaderProgram {
  
private:
  std::string name_;

protected:
  QOpenGLContext* gl_;
  GLuint program_;
  GLuint vert_shader_;
  GLuint frag_shader_;

  inline ShaderProgram(QOpenGLContext* gl, std::string name) :
    gl_(gl), name_(name) {
    
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
    char* logbuf = new char[256];
    auto gf = gl_->functions();
    vert_shader_ = gf->glCreateShader(GL_VERTEX_SHADER);
    frag_shader_ = gf->glCreateShader(GL_FRAGMENT_SHADER);
    gf->glShaderSource(vert_shader_, 1, &vert_shader_source, NULL);
    gf->glShaderSource(frag_shader_, 1, &frag_shader_source, NULL);

    gf->glCompileShader(vert_shader_);
    gf->glGetShaderInfoLog(vert_shader_, 256, nullptr, logbuf);
    spdlog::get(LOGGER_DEFAULT)->info("[ShaderProgram] Compile vert shader {} => {}", name_, logbuf);
    gf->glCompileShader(frag_shader_);
    gf->glGetShaderInfoLog(frag_shader_, 256, nullptr, logbuf);
    spdlog::get(LOGGER_DEFAULT)->info("[ShaderProgram] Compile frag shader {} => {}", name_, logbuf);


    program_ = gf->glCreateProgram();
    gf->glAttachShader(program_, vert_shader_);
    gf->glAttachShader(program_, frag_shader_);
    gf->glLinkProgram(program_);
    gf->glGetProgramInfoLog(program_, 256, nullptr, logbuf);
    spdlog::get(LOGGER_DEFAULT)->info("[ShaderProgram] Link program {} => {}", name_, logbuf);
    delete logbuf;
  } 

public:
  inline GLuint id() const {
    return program_;
  }

};

}

}

#endif