#ifndef NLIVE_AUDIO_RENDERER_RENDERER_H_
#define NLIVE_AUDIO_RENDERER_RENDERER_H_

extern "C" {
  #include <libswresample/swresample.h>
}
#include "base/common/memory.h"
#include <QThread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "base/common/memory.h"
#include "model/common/rational.h"
#include "renderer/audio_renderer/render_context.h"
#include "renderer/audio_renderer/render_state.h"
#include "renderer/audio_renderer/command_buffer.h"
#include "renderer/audio_renderer/render_io.h"

#include <cassert>

namespace nlive {

namespace audio_renderer {

class Renderer : public QThread {
  Q_OBJECT

private:
  enum State {
    IDLE,
    RESET,
    WAITING_DATA,
    DATA_AVAILABLE,
    CLOSED
  };

  int state_;
  int sample_rate_;
  int samples_per_channel_;
  sptr<RenderState> render_state_;
  sptr<RenderContext> render_context_;
  sptr<RenderIO> render_io_;
  std::mutex state_mutex_;
  std::condition_variable state_cv_;

  int writing_index_;
  sptr<CommandBuffer> requested_command_buffer_;
  sptr<CommandBuffer> requested_burst_command_buffer_;

protected:
  void run() override;

public:
  Renderer(int64_t ch_layout, AVSampleFormat sample_fmt, int sample_rate,
    int samples_per_channel, int kernels_per_slot, int slot_length);
  ~Renderer();
  
  // void start();
  void reset();
  // void restart();
  void close();
  void sendRenderCommandBuffer(sptr<CommandBuffer> command_buffer, int index);
  void sendBurstRenderCommandBuffer(sptr<CommandBuffer> command_buffer);

  int64_t calculateFrameByIndex(int index) const;

  inline sptr<RenderContext> render_context() { return render_context_; }

  inline int sample_rate() const { return sample_rate_; }

signals:
  void onRenderRequest(int index, qint64 start_frame, qint64 end_frame);


};

}

}

#endif