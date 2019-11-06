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
  QSharedPointer<TextureShaderProgram> texture_shader =
    QSharedPointer<TextureShaderProgram>(new TextureShaderProgram(gl_));
  QSharedPointer<SolidShaderProgram> solid_shader =
    QSharedPointer<SolidShaderProgram>(new SolidShaderProgram(gl_));
  QSharedPointer<MirrorShaderProgram> mirror_shader =
    QSharedPointer<MirrorShaderProgram>(new MirrorShaderProgram(gl_));
  QSharedPointer<GrayScaleShaderProgram> gray_scale_shader =
    QSharedPointer<GrayScaleShaderProgram>(new GrayScaleShaderProgram(gl_));
  registerShaderProgram("texture_shader", texture_shader);
  registerShaderProgram("solid_shader", solid_shader);
  registerShaderProgram("mirror_shader", mirror_shader);
  registerShaderProgram("gray_scale_shader", gray_scale_shader);
}

void RenderSharingContext::registerShaderProgram(
    std::string name, QSharedPointer<ShaderProgram> program) {
  shaders_.insert(make_pair(name, program));
}

QSharedPointer<ShaderProgram> RenderSharingContext::getShaderProgram(std::string name) {
  Q_ASSERT(shaders_.count(name));
  return shaders_[name];
}

}

}