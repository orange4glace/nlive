#ifndef NLIVE_AUDIO_RENDERER_RENDER_CONTEXT_H_
#define NLIVE_AUDIO_RENDERER_RENDER_CONTEXT_H_

namespace nlive {

namespace audio_renderer {

class RenderContext {

private:
  int sample_rate_;
  int buffer_size_;
  int byte_per_sample_;
  int sample_per_buffer_;
  void* data_;

public:
  RenderContext(int sample_rate, int buffer_size);

  void clearData();
  inline int byte_per_sample() { return byte_per_sample_; }
  inline int buffer_size() { return buffer_size_; }
  inline int sample_per_buffer() { return sample_per_buffer_; }
  inline void* data() { return data_; }

};

}

}

#endif