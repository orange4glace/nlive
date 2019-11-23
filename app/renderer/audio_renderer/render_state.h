#ifndef NLIVE_AUDIO_RENDERER_RENDER_STATE_H_
#define NLIVE_AUDIO_RENDERER_RENDER_STATE_H_

#include <QElapsedTimer>
#include "base/common/memory.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include "base/common/memory.h"
#include "renderer/audio_renderer/ring_buffer.h"

namespace nlive {

namespace audio_renderer {

class RenderState {

public:
  enum State {

  };

private:
  int nb_channels_;
  int bytes_per_sample_;
  int sample_rate_;
  int samples_per_channel_;
  int samples_per_kernel_;
  int kernels_per_slot_;
  int slot_length_;

  sptr<RingBuffer<uint8_t>> buffer_;
  std::mutex state_mutex_;
  std::condition_variable state_cv_;
  std::vector<std::mutex*> slot_mutexes_;
  std::mutex mutex_;

  int producer_index_;
  int consumer_index_;
  bool producer_wait_flag_;

  QElapsedTimer timer_;

public:
  RenderState(int nb_channels, int bytes_per_sample, int sample_rate,
    int samples_per_channel, int kernels_per_slot, int slot_length);

  void reset();

  inline std::mutex& state_mutex() { return state_mutex_; }
  inline std::condition_variable& state_cv() { return state_cv_; }
  inline std::mutex& slot_mutex_at(int index) {
    int offset = index % slot_length_;
    return *slot_mutexes_[offset];
  }
  inline int64_t elapsed_utime() { return timer_.nsecsElapsed(); }

  inline int producer_index() { return producer_index_; }
  inline void setProducerIndex(int value) { producer_index_ = value; }
  inline int consumer_index() { return consumer_index_; }
  inline void setConsumerIndex(int value) { consumer_index_ = value; }

  inline bool producer_wait_flag() { return producer_wait_flag_; }
  inline void setProducerWaitFlag(int value) { producer_wait_flag_ = value; }
  
  inline int nb_channels() const { return nb_channels_; }
  inline int bytes_per_sample() const { return bytes_per_sample_; }
  inline int sample_rate() const { return sample_rate_; }
  inline int samples_per_kernel() const { return samples_per_kernel_; }
  inline int kernels_per_slot() const { return kernels_per_slot_; }
  inline int slot_length() const { return slot_length_; }

  inline sptr<RingBuffer<uint8_t>> buffer() { return buffer_; }

};

}

}

#endif