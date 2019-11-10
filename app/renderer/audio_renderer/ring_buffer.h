#ifndef NLIVE_AUDIO_RENDERER_RING_BUFFER_H_
#define NLIVE_AUDIO_RENDERER_RING_BUFFER_H_

#include <QDebug>
#include <cstring>

namespace nlive {

namespace audio_renderer {

template <typename T>
class RingBuffer {

private:
  int kernel_size_;
  int kernel_length_per_slot_;
  int slot_length_;
  int buffer_size_;
  int bytes_per_slot_;
  T* buffer_;

public:
  RingBuffer(int kernel_size, int kernel_length_per_slot, int slot_length) :
    kernel_size_(kernel_size), kernel_length_per_slot_(kernel_length_per_slot),
    slot_length_(slot_length) {
    // TODO : generalize byte size by type
    int byte_per_sample = 4;
    bytes_per_slot_ = kernel_size * kernel_length_per_slot * byte_per_sample;
    buffer_size_ = bytes_per_slot_ * slot_length;
    buffer_ = new T[kernel_size * kernel_length_per_slot * slot_length];
  }

  void copyFrom(int slot_index, void* src) {
    int slot_offset = slot_index % slot_length_;
    void* slot_addr = buffer_ + (kernel_size_ * kernel_length_per_slot_) * slot_offset;
    memcpy(slot_addr, src, bytes_per_slot_);
  }

  void copyTo(int slot_index, void* dst) {
    int slot_offset = slot_index % slot_length_;
    void* slot_addr = buffer_ + (kernel_size_ * kernel_length_per_slot_) * slot_offset;
    memcpy(dst, slot_addr, bytes_per_slot_);
  }

  T* getSlot(int index) {
    int offset = index % slot_length_;
    return (buffer_ + (kernel_size_ * kernel_length_per_slot_) * offset);
  }

  int kernel_size() const { return kernel_size_; }
  int kernel_length_per_slot() const { return kernel_length_per_slot_; }
  int slot_length() const { return slot_length_; }
  int bytes_per_slot() const { return bytes_per_slot_; }
  int buffer_size() const { return buffer_size_; }

};

}

}

#endif