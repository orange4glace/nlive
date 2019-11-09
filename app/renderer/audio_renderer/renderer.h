#ifndef NLIVE_AUDIO_RENDERER_RENDERER_H_
#define NLIVE_AUDIO_RENDERER_RENDERER_H_

#include <QSharedPointer>
#include <QThread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "base/common/memory.h"
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
  };

  int state_;
  int sample_rate_;
  sptr<RenderState> render_state_;
  QSharedPointer<RenderContext> render_context_;
  sptr<RenderIO> render_io_;
  std::mutex state_mutex_;
  std::condition_variable state_cv_;

  int writing_index_;
  QSharedPointer<CommandBuffer> requested_command_buffer_;

protected:
  void run() override;

public:
  Renderer(int sample_rate, int kernel_size, int kernel_length_per_slot,
    int slot_length);
  
  void reset();
  void sendRenderCommandBuffer(QSharedPointer<CommandBuffer> command_buffer, int index);

  inline QSharedPointer<RenderContext> render_context() { return render_context_; }

signals:
  void onRenderRequest(int index);


};

}

}

#endif