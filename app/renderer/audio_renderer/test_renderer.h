#ifndef NLIVE_AUDIO_RENDERER_TEST_RENDERER_H_
#define NLIVE_AUDIO_RENDERER_TEST_RENDERER_H_

#include <QObject>
#include <QTimer>
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
    
    QTimer* t = new QTimer(this);
    t->setInterval(1000);
    connect(t, &QTimer::timeout, this, [this]() {
      QSharedPointer<CommandBuffer> rb = QSharedPointer<CommandBuffer>(
        new CommandBuffer());
      QSharedPointer<NoiseRenderCommand> nrc2 = QSharedPointer<NoiseRenderCommand>(
        new NoiseRenderCommand());
      rb->addCommand(nrc2);
      renderer_->sendBurstRenderCommandBuffer(rb);
    });
    t->start();

    renderer_ = new Renderer(AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_FLT, 88200, 88200/30, 1, 8);
    connect(renderer_, &Renderer::onRenderRequest, this, [this, buffer](int index, int start_frame, int end_frame) {
      QSharedPointer<CommandBuffer> rb = QSharedPointer<CommandBuffer>(
        new CommandBuffer());
      int offset = index * renderer_->render_context()->bytes_per_channel();
      int length = renderer_->render_context()->bytes_per_channel();
      int start_sample = Rational::rescale(44100, start_frame, renderer_->sample_rate());
      int end_sample = Rational::rescale(44100, end_frame, renderer_->sample_rate());
      qDebug() << "[TestAudioRenderer]" << start_frame << end_frame << start_sample << end_sample;
      QSharedPointer<PCMRenderCommand> nrc1 = QSharedPointer<PCMRenderCommand>(
        new PCMRenderCommand(AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_FLT, 44100, end_sample - start_sample, (uint8_t*)buffer, start_sample * 4));
      QSharedPointer<NoiseRenderCommand> nrc2 = QSharedPointer<NoiseRenderCommand>(
        new NoiseRenderCommand());
      rb->addCommand(nrc1);
      // rb->addCommand(nrc2);
      renderer_->sendRenderCommandBuffer(rb, index);
    }, Qt::QueuedConnection);
    renderer_->start();
  }

};

}

}

#endif