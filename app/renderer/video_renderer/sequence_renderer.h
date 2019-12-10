#ifndef NLIVE_VIDEO_RENDERER_SEQUENCE_RENDERER_H_
#define NLIVE_VIDEO_RENDERER_SEQUENCE_RENDERER_H_

#include <QObject>
#include "base/common/memory.h"
#include <QOffscreenSurface>

#include "base/common/sig.h"
#include "renderer/video_renderer/render_thread.h"
#include "model/sequence/sequence.h"

namespace nlive {

namespace video_renderer {

class CommandBuffer;

class SequenceRenderer : public QObject, public Sig {
  Q_OBJECT

private:
  sptr<Sequence> sequence_;
  QOpenGLContext* target_gl_;

  sptr<RenderThread> renderer_;

public:
  SequenceRenderer(
    sptr<Sequence> sequence,
    QOpenGLContext* target_gl);
  ~SequenceRenderer();

  // std::unique_ptr<RenderTexture> getRenderData();
  sptr<Sequence> sequence();
  void initialize();
  void paint();
  void render(int64_t timecode);

signals:
  void onDidReadyData();

};

}

}

#endif