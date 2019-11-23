#include "decoder/audio_decoder.h"

#include <QDebug>

namespace nlive {

AudioDecoder::AudioDecoder(sptr<RawAudioResource> resource) :
  resource_(resource), file_(QString::fromStdString(resource->path())) {
  file_.open(QIODevice::ReadOnly);

  frame_ = sptr<AudioFrame>(new AudioFrame(
    resource->nb_channels(), resource->bytes_per_sample()));
}

sptr<AudioFrame> AudioDecoder::decode(int64_t start_frame, int64_t end_frame) {
  int64_t a_start_frame = std::max(0LL, start_frame);
  int64_t start_byte = a_start_frame * resource_->bytes_per_sample() * resource_->nb_channels();
  int64_t end_byte = end_frame * resource_->bytes_per_sample() * resource_->nb_channels();
  int64_t byte_offset = (a_start_frame - start_frame) * resource_->bytes_per_sample() * resource_->nb_channels();
  int64_t byte_length = end_byte - start_byte;
  frame_->setFrameSize(end_frame - start_frame);
  frame_->clear();
  // TODO : read barrier (overflow max file length)
  file_.seek(start_byte);
  file_.read((char*)(frame_->data() + byte_offset), byte_length);
  return frame_;
}


}