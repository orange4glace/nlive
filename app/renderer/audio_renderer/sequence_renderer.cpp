#include "renderer/audio_renderer/sequence_renderer.h"

namespace nlive {

namespace audio_renderer {

SequenceRenderer::SequenceRenderer(
  sptr<Sequence> sequence) :
  sequence_(sequence), playing_(false) {
  renderer_ = sptr<Renderer>(
    new Renderer(AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_FLT, sequence->sample_rate(),
    sequence->sample_rate() / 30, 1, 8));
  connect(renderer_.get(), &Renderer::onRenderRequest, this, [this](int index, int64_t start_frame, int64_t end_frame) {
    playingCallback(index, start_frame, end_frame);
  });
  renderer_->start();
}


void SequenceRenderer::playingCallback(int index, int64_t start_frame, int64_t end_frame) {
  if (!playing_) return;
  start_frame += base_frame_;
  end_frame += base_frame_;
  auto command_buffer = sequence_->renderAudio(start_frame, end_frame);
  renderer_->sendRenderCommandBuffer(command_buffer, index);
}

void SequenceRenderer::start() {
  renderer_->reset();
  int64_t current_time = sequence_->current_time();
  base_frame_ = Rational::rescale(current_time, sequence_->time_base(), Rational(1, sequence_->sample_rate()));
  playing_ = true;
}

void SequenceRenderer::stop() {
  playing_ = false;
  renderer_->reset();
}

void SequenceRenderer::render() {
  int64_t current_time = sequence_->current_time();
  int64_t start_frame = Rational::rescale(current_time, sequence_->time_base(), Rational(1, sequence_->sample_rate()));
  int64_t end_frame = start_frame + renderer_->render_context()->samples_per_channel();
  auto command_buffer = sequence_->renderAudio(start_frame, end_frame);
  renderer_->sendBurstRenderCommandBuffer(command_buffer);
}

}

}