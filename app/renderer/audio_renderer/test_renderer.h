#ifndef NLIVE_AUDIO_RENDERER_TEST_RENDERER_H_
#define NLIVE_AUDIO_RENDERER_TEST_RENDERER_H_

#include <QObject>
#include <QDebug>
#include "renderer/audio_renderer/renderer.h"
#include "renderer/audio_renderer/command/pcm_render_command.h"
#include "renderer/audio_renderer/command/noise_render_command.h"

#include <fstream>

namespace nlive {

namespace audio_renderer {

class TestRenderer : public QObject {
  Q_OBJECT

private:
  Renderer* renderer_;

public:
  inline TestRenderer(QObject* parent) :
    QObject(parent) {
    std::ifstream infile("output.wav");
    //get length of file
    infile.seekg(0, std::ios::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);
    char* buffer = new char[length];
    infile.read(buffer, length);

    renderer_ = new Renderer(44100, 3000, 1, 8);
    connect(renderer_, &Renderer::onRenderRequest, this, [this, buffer](int index) {
      QSharedPointer<CommandBuffer> rb = QSharedPointer<CommandBuffer>(
        new CommandBuffer());
      int offset = index * renderer_->render_context()->buffer_size();
      int length = renderer_->render_context()->buffer_size();
      QSharedPointer<PCMRenderCommand> nrc = QSharedPointer<PCMRenderCommand>(
        new PCMRenderCommand(buffer, offset, length));
      rb->addCommand(nrc);
      renderer_->sendRenderCommandBuffer(rb, index);
    }, Qt::QueuedConnection);
    renderer_->start();
  }

};

}

}

#endif