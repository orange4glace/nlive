#ifndef NLIVE_RENDERER_CONTEXT_H_
#define NLIVE_RENDERER_CONTEXT_H_

#include <map>
#include <vector>
#include <string>
#include <QOpenGLContext>
#include <QSharedPointer>
#include "renderer/video_renderer/render_sharing_context.h"

namespace nlive {

namespace video_renderer {

struct RenderTexture {
  int id;
  std::string name;
  int width, height;
  GLuint framebuffer;
  GLuint texture;
};

class RendererContext {

private:
  QOpenGLContext* gl_;
  int width_, height_;
  std::map<std::string, RenderTexture> render_textures_;
  std::vector<RenderTexture> swap_buffers_;
  int front_buffer_index_;

  std::map<std::string, GLuint> shaders_;

  QSharedPointer<RenderSharingContext> sharing_context_;

  bool initialized_;

public:
  RendererContext(QOpenGLContext* gl, int width, int height,
      QSharedPointer<RenderSharingContext> sharing_context = nullptr);

  void initialize();

  RenderTexture createTemporaryRenderTexture(
    std::string name,
    int width,
    int height);
  RenderTexture getTemporaryRenderTexture(std::string name);
  void releaseTemporaryRenderTexture(RenderTexture& rt);
  void releaseTemporaryRenderTexture(std::string name);
  void swapTemporaryRenderTexture(std::string lhs, std::string rhs);
  RenderTexture getFrontRenderTexture();
  RenderTexture getBackRenderTexture();
  // void blit(RenderTexture& rt);
  // int getProgram(const char* name);
  void swapRenderTextures();

  int width() const;
  int height() const;
  QSharedPointer<RenderSharingContext> sharing_context();
  bool initialized() const;
  
  QOpenGLContext* gl();

};

}

}

#endif