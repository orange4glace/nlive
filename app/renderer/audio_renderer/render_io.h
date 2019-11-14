#ifndef NLIVE_AUDIO_RENDERER_RENDER_IO_H_
#define NLIVE_AUDIO_RENDERER_RENDER_IO_H_

#include <QThread>
#include <QAudioOutput>
#include <QIODevice>
#include "base/common/memory.h"
#include "renderer/audio_renderer/render_state.h"

namespace nlive {

namespace audio_renderer {

class RenderDevice : public QIODevice {
  Q_OBJECT

private:
  sptr<RenderState> render_state_;
  int buffer_size_;

  int64_t byte_index_;

protected:
  qint64 readData(char* data, qint64 max_size) override;
  qint64 writeData(const char* data, qint64 max_size) override;

public:
  RenderDevice(QObject* parent, sptr<RenderState> render_state);

};

class RenderIO : public QThread {
  Q_OBJECT

private:
  sptr<RenderState> render_state_;
  QAudioOutput* output_;
  RenderDevice* device_;

protected:
  void run() override;

public:
  RenderIO(QObject* parent, sptr<RenderState> render_state);

};

}

}

#endif