#ifndef NLIVE_AUDIO_FRAME_H_
#define NLIVE_AUDIO_FRAME_H_

#include <QDebug>
extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

namespace nlive {

class AudioFrame {

private:
  uint8_t* data_;
  int64_t buffer_size_;
  int64_t frame_size_;
  int64_t reserved_frame_size_;
  int nb_channels_;
  int bytes_per_sample_;

  inline void reserve(int frame_size) {
    if (frame_size <= reserved_frame_size_) return;
    if (data_) delete data_;
    reserved_frame_size_ = frame_size + 1024;
    buffer_size_ = reserved_frame_size_ * bytes_per_sample_ * nb_channels_;
    data_ = new uint8_t[buffer_size_];
  }

public:
  inline AudioFrame(int nb_channels, int bytes_per_sample) :
    data_(nullptr), nb_channels_(nb_channels), bytes_per_sample_(bytes_per_sample) {
    reserved_frame_size_ = 0;
  }

  inline ~AudioFrame() {
    if (data_) delete data_;
  }

  inline void clear() {
    memset(data_, 0, buffer_size_);
  }

  inline void setFrameSize(int64_t frame_size) {
    reserve(frame_size);
    frame_size_ = frame_size;
  }

  inline uint8_t* data() { return data_; }
  inline int64_t frame_size() const { return frame_size_; }

};

}

#endif