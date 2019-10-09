#ifndef NLIVE_RENDERER_CONTEXT_H_
#define NLIVE_RENDERER_CONTEXT_H_

#include <map>
#include <vector>
#include <QOpenGLContext>
#include <QSharedPointer>

namespace nlive {

namespace video_renderer {

struct RenderTexture {
  int id;
  GLuint framebuffer;
  GLuint texture;
};

class RendererContext {

private:
  QOpenGLContext* gl_;
  std::map<int, RenderTexture> render_textures;
  std::vector<RenderTexture> swap_buffers_;
  int front_buffer_index_;

  bool initialized_;

public:
  RendererContext(QOpenGLContext* gl);

  void initialize();

  RenderTexture getTemporaryRenderTexture();
  void releaseTemporaryRenderTexture(RenderTexture& rt);
  RenderTexture getFrontRenderTexture();
  RenderTexture getBackRenderTexture();
  // void blit(RenderTexture& rt);
  // int getProgram(const char* name);
  void swapRenderTextures();

  bool initialized() const;
  
  QOpenGLContext* gl();

};

}

}

#endif