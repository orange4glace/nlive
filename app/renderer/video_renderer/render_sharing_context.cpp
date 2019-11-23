#include "renderer/video_renderer/render_sharing_context.h"

#include "renderer/video_renderer/shader/solid_shader.h"
#include "renderer/video_renderer/shader/texture_shader.h"
#include "renderer/video_renderer/shader/mirror_shader.h"
#include "renderer/video_renderer/shader/gray_scale_shader.h"

namespace nlive {

namespace video_renderer {

RenderSharingContext::RenderSharingContext(QOpenGLContext* gl) :
    gl_(gl) {
}

void RenderSharingContext::initialize() {
  sptr<TextureShaderProgram> texture_shader =
    sptr<TextureShaderProgram>(new TextureShaderProgram(gl_));
  sptr<SolidShaderProgram> solid_shader =
    sptr<SolidShaderProgram>(new SolidShaderProgram(gl_));
  sptr<MirrorShaderProgram> mirror_shader =
    sptr<MirrorShaderProgram>(new MirrorShaderProgram(gl_));
  sptr<GrayScaleShaderProgram> gray_scale_shader =
    sptr<GrayScaleShaderProgram>(new GrayScaleShaderProgram(gl_));
  registerShaderProgram("texture_shader", texture_shader);
  registerShaderProgram("solid_shader", solid_shader);
  registerShaderProgram("mirror_shader", mirror_shader);
  registerShaderProgram("gray_scale_shader", gray_scale_shader);
}

void RenderSharingContext::registerShaderProgram(
    std::string name, sptr<ShaderProgram> program) {
  shaders_.insert(make_pair(name, program));
}

sptr<ShaderProgram> RenderSharingContext::getShaderProgram(std::string name) {
  Q_ASSERT(shaders_.count(name));
  return shaders_[name];
}

}

}