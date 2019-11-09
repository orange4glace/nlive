#ifndef NLIVE_AUDIO_RENDERER_RENDER_STATE_H_
#define NLIVE_AUDIO_RENDERER_RENDER_STATE_H_

#include <QSharedPointer>
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


  sptr<RingBuffer<float>> buffer_;
  std::mutex state_mutex_;
  std::condition_variable state_cv_;
  std::vector<std::mutex*> slot_mutexes_;
  std::mutex mutex_;
  int sample_rate_;
  int slot_length_;

  int producer_index_;
  int consumer_index_;
  bool producer_wait_flag_;

public:
  RenderState(int sample_rate, int kernel_size, int kernel_length_per_slot,
    int slot_length);

  void reset();

  inline std::mutex& state_mutex() { return state_mutex_; }
  inline std::condition_variable& state_cv() { return state_cv_; }
  inline std::mutex& slot_mutex_at(int index) {
    int offset = index % slot_length_;
    return *slot_mutexes_[offset];
  }

  inline int producer_index() { return producer_index_; }
  inline void setProducerIndex(int value) { producer_index_ = value; }
  inline int consumer_index() { return consumer_index_; }
  inline void setConsumerIndex(int value) { consumer_index_ = value; }

  inline bool producer_wait_flag() { return producer_wait_flag_; }
  inline void setProducerWaitFlag(int value) { producer_wait_flag_ = value; }
  
  inline int sample_rate() const { return sample_rate_; }
  inline int kernel_size() const { return buffer_->kernel_size(); }
  inline int kernel_length_per_slot() const { return buffer_->kernel_length_per_slot(); }
  inline int slot_length() const { return buffer_->slot_length(); }
  inline int bytes_per_slot() const { return buffer_->bytes_per_slot(); }
  inline int buffer_size() const { return buffer_->buffer_size(); }

  inline sptr<RingBuffer<float>> buffer() { return buffer_; }

};

}

}

#endif