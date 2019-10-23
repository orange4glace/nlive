#ifndef _NLIVE_VIDEO_RENDER_COMMAND_H_
#define _NLIVE_VIDEO_RENDER_COMMAND_H_

#include <QSharedPointer>
#include <QOpenGLFunctions>
#include <stdint.h>

#include "renderer/video_renderer/render_command.h"
#include "decoder/video_decoder.h"
#include "model/resource/video_resource.h"

namespace nlive {

namespace video_renderer {

class VideoRenderCommand : public RenderCommand {

private:
  QSharedPointer<VideoResource> resource_;
  QSharedPointer<VideoDecoder> decoder_;
  int64_t timestamp_;

  uint8_t* buffer_;
  bool is_external_buffer_;

  bool initialized_;
  GLuint vert_shader_;
  GLuint frag_shader_;
  GLuint program_;
  GLuint position_loc_;
  GLuint texCoord_loc_;
  GLuint image_loc_;
  GLuint position_buffer_;
  GLuint texCoord_buffer_;
  GLuint tex_;

  inline void initialize(QOpenGLContext* gl) {
    if (initialized_) return;
    initialized_ = true;
    auto gf = gl->functions();
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

public:
  inline VideoRenderCommand(
    QSharedPointer<VideoResource> resource,
    QSharedPointer<VideoDecoder> decoder,
    int64_t timestamp) :
    resource_(resource), decoder_(decoder), timestamp_(timestamp),
    buffer_(nullptr), is_external_buffer_(false), initialized_(false) {
  }

  inline ~VideoRenderCommand() {
    if (buffer_ && !is_external_buffer_) delete buffer_;
  }

  inline void render(QSharedPointer<RendererContext> ctx) {
    QOpenGLContext* gl = ctx->gl();
    decoder_->open();
    QSharedPointer<VideoFrame> video_frame = decoder_->decode(timestamp_);
    qDebug() << "FRAME = " << video_frame << "\n";
    // TODO: If video_frame == NULL
    if (!video_frame) return;
    uint8_t* buf = new uint8_t[resource_->width() * resource_->height() * 4];
    video_frame->scale((void*)buf);
    auto gf = gl->functions();
    RenderTexture rt = ctx->getBackRenderTexture();
    
    gf->glBindTexture(GL_TEXTURE_2D, rt.texture);
    gf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resource_->width(), resource_->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
    gf->glBindTexture(GL_TEXTURE_2D, 0);

    delete buf;
  }

};

}

}

#endif