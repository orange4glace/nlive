#ifndef NLIVE_VIDEO_RENDERER_RENDERER_CONTEXT_H_
#define NLIVE_VIDEO_RENDERER_RENDERER_CONTEXT_H_

#include <map>
#include <vector>
#include <string>
#include <QOpenGLContext>
#include <QSharedPointer>
#include "renderer/video_renderer/render_sharing_context.h"
#include "renderer/video_renderer/decoder_manager.h"

namespace nlive {

namespace video_renderer {

struct SwsKey {
  int src_width;
  int src_height;
  int out_width;
  int out_height;
  AVPixelFormat src_fmt;
};

// TODO : Create hash to compare
struct SwsKeyCompare {
  inline bool operator() (const SwsKey& a, const SwsKey& b) const {
    if (a.src_width == b.src_width) {
      if (a.src_height == b.src_height) {
        if (a.out_width == b.out_width) {
          if (a.out_height == b.out_height) {
            return a.src_fmt < b.src_fmt;
          }
          return a.out_height < b.out_height;
        }
        return a.out_width < b.out_width;
      }
      return a.src_height < b.src_height;
    }
    return a.src_width < b.src_width;
  }
};

struct SwsValue {
  SwsContext* context;
  uint8_t* out_data;
};

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


  QSharedPointer<RenderSharingContext> sharing_context_;
  QSharedPointer<DecoderManager> decoder_manager_;
  std::map<SwsKey, SwsValue, SwsKeyCompare> sws_map_;

  bool initialized_;

public:
  RendererContext(QOpenGLContext* gl, int width, int height,
      QSharedPointer<RenderSharingContext> sharing_context = nullptr);
  ~RendererContext();

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
  SwsValue* getSwsValue(SwsKey& key);

  int width() const;
  int height() const;
  QSharedPointer<RenderSharingContext> sharing_context();
  QSharedPointer<DecoderManager> decoder_manager();
  bool initialized() const;
  
  QOpenGLContext* gl();

};

}

}

#endif