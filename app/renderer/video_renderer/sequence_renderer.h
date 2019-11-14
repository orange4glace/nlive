#ifndef NLIVE_VIDEO_RENDERER_SEQUENCE_RENDERER_H_
#define NLIVE_VIDEO_RENDERER_SEQUENCE_RENDERER_H_

#include <QObject>
#include <QSharedPointer>
#include <QOffscreenSurface>

#include "base/common/sig.h"
#include "renderer/video_renderer/renderer.h"
#include "model/sequence/sequence.h"

namespace nlive {

namespace video_renderer {

class CommandBuffer;

class SequenceRenderer : public QObject, public Sig {
  Q_OBJECT

private:
  QSharedPointer<Sequence> sequence_;
  QOpenGLContext* target_gl_;

  QSharedPointer<Renderer> renderer_;

public:
  SequenceRenderer(
    QSharedPointer<Sequence> sequence,
    QOpenGLContext* target_gl);
  ~SequenceRenderer();

  // std::unique_ptr<RenderTexture> getRenderData();
  void initialize();
  void paint();
  void render();

signals:
  void onDidReadyData();

};

}

}

#endif