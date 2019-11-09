#include "renderer/audio_renderer/render_context.h"

#include <cstring>
#include <stdint.h>

namespace nlive {

namespace audio_renderer {

RenderContext::RenderContext(int sample_rate, int buffer_size) :
  sample_rate_(sample_rate), buffer_size_(buffer_size) {
  byte_per_sample_ = 4;
  sample_per_buffer_ = buffer_size / byte_per_sample_;
  data_ = new uint8_t[buffer_size];
  clearData();
}

void RenderContext::clearData() {
  memset(data_, 0, buffer_size_);
}

}

}