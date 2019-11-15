#include "renderer/audio_renderer/render_state.h"

namespace nlive {

namespace audio_renderer {

RenderState::RenderState(int nb_channels, int bytes_per_sample, int sample_rate,
    int samples_per_channel, int kernels_per_slot, int slot_length) :
  nb_channels_(nb_channels), bytes_per_sample_(bytes_per_sample), sample_rate_(sample_rate),
  samples_per_channel_(samples_per_channel), kernels_per_slot_(kernels_per_slot), slot_length_(slot_length) {
  samples_per_kernel_ = nb_channels * samples_per_channel;
  buffer_ = std::make_shared<RingBuffer<uint8_t>>
    (nb_channels, samples_per_channel, bytes_per_sample, kernels_per_slot, slot_length);
  for (int i = 0; i < slot_length; i ++) {
    std::mutex* m = new std::mutex();
    slot_mutexes_.push_back(m);
  }
  reset();
}

void RenderState::reset() {
  producer_index_ = 0;
  consumer_index_ = 0;
  producer_wait_flag_ = false;
  timer_.restart();
  state_cv_.notify_all();
}

}

}