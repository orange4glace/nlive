#ifndef NLIVE_VIDEO_RENDERER_RENDER_SHARING_CONTEXT_H_
#define NLIVE_VIDEO_RENDERER_RENDER_SHARING_CONTEXT_H_

#include <QSharedPointer>
#include <QOpenGLContext>
#include "renderer/video_renderer/shader_program.h"

namespace nlive {

namespace video_renderer {

class RenderSharingContext {

private:
  QOpenGLContext* gl_;
  std::map<std::string, QSharedPointer<ShaderProgram>> shaders_;

public:
  RenderSharingContext(QOpenGLContext* gl);

  void initialize();

  void registerShaderProgram(std::string name, QSharedPointer<ShaderProgram> program);
  QSharedPointer<ShaderProgram> getShaderProgram(std::string name);

};

}

}

#endif