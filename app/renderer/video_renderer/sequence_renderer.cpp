#include "renderer/video_renderer/sequence_renderer.h"

#include <QDebug>

namespace nlive {

namespace video_renderer {

SequenceRenderer::SequenceRenderer(
  QSharedPointer<Sequence> sequence,
  QOpenGLContext* target_gl) :
  sequence_(sequence),
  target_gl_(target_gl) {
  renderer_ = QSharedPointer<Renderer>(new Renderer(target_gl, sequence->width(), sequence->height()));
  
  connect(sequence_.get(), &Sequence::onDirty, this, &SequenceRenderer::render);
  connect(renderer_.get(), &Renderer::onDidReadyData, this, [this]() {
    emit onDidReadyData();
  });

  renderer_->start();
}

std::unique_ptr<RenderTexture> SequenceRenderer::getRenderData() {
  return std::move(renderer_->getRenderData());
}

void SequenceRenderer::render(QSharedPointer<CommandBuffer> command_buffer) {
  renderer_->render(command_buffer);
}

}

}