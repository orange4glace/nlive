#ifndef NLIVE_AUDIO_RENDERER_RING_BUFFER_H_
#define NLIVE_AUDIO_RENDERER_RING_BUFFER_H_

#include <QDebug>
#include <cstring>

namespace nlive {

namespace audio_renderer {

template <typename T>
class RingBuffer {

private:
  int nb_channels_;
  int samples_per_channel_;
  int bytes_per_sample_;
  int bytes_per_channel_;
  int bytes_per_kernel_;
  int kernels_per_slot_;
  int slot_length_;
  int buffer_size_;
  int bytes_per_slot_;
  T* buffer_;

public:
  RingBuffer(int nb_channels, int samples_per_channel, int bytes_per_sample, int kernels_per_slot, int slot_length) :
    nb_channels_(nb_channels), samples_per_channel_(samples_per_channel),
    bytes_per_sample_(bytes_per_sample), kernels_per_slot_(kernels_per_slot), slot_length_(slot_length) {
    bytes_per_channel_ = samples_per_channel * bytes_per_sample;
    bytes_per_kernel_ = nb_channels * bytes_per_channel_; 
    bytes_per_slot_ = bytes_per_kernel_ * kernels_per_slot;
    buffer_size_ = bytes_per_slot_ * slot_length;
    buffer_ = new T[buffer_size_];
  }

  void copyFrom(int slot_index, uint8_t* src) {
    int slot_offset = slot_index % slot_length_;
    uint8_t* slot_addr = buffer_ + bytes_per_slot_ * slot_offset;
    memcpy(slot_addr, src, bytes_per_slot_);
  }

  void copyTo(int slot_index, uint8_t* dst) {
    int slot_offset = slot_index % slot_length_;
    uint8_t* slot_addr = buffer_ + bytes_per_slot_ * slot_offset;
    memcpy(dst, slot_addr, bytes_per_slot_);
  }

  void copyTo(int slot_index, uint8_t* dst, int offset, int length) {
    int slot_offset = slot_index % slot_length_;
    uint8_t* slot_addr = buffer_ + bytes_per_slot_ * slot_offset;
    memcpy(dst, slot_addr + offset, length);
  }

  T* getSlot(int index) {
    int offset = index % slot_length_;
    return (buffer_ + (bytes_per_kernel_ * kernels_per_slot_) * offset);
  }

  int bytes_per_kernel() const { return bytes_per_kernel_; }
  int kernels_per_slot() const { return kernels_per_slot_; }
  int slot_length() const { return slot_length_; }
  int bytes_per_slot() const { return bytes_per_slot_; }
  int buffer_size() const { return buffer_size_; }

};

}

}

#endif