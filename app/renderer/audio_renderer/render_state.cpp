#include "renderer/audio_renderer/render_state.h"

namespace nlive {

namespace audio_renderer {

RenderState::RenderState(int sample_rate, int kernel_size, int kernel_length_per_slot,
    int slot_length) :
  sample_rate_(sample_rate), slot_length_(slot_length) {
  buffer_ = std::make_shared<RingBuffer<float>>
    (kernel_size, kernel_length_per_slot, slot_length);
  producer_index_ = 0;
  consumer_index_ = 0;
  for (int i = 0; i < slot_length; i ++) {
    std::mutex* m = new std::mutex();
    slot_mutexes_.push_back(m);
  }
}

void RenderState::reset() {
  producer_index_ = 0;
  consumer_index_ = 0;
  producer_wait_flag_ = false;
  state_cv_.notify_all();
}

}

}