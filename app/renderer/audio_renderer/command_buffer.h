#ifndef NLIVE_AUDIO_RENDERER_COMMAND_BUFFER_H_
#define NLIVE_AUDIO_RENDERER_COMMAND_BUFFER_H_

#include <vector>
#include <QSharedPointer>

#include "renderer/audio_renderer/render_command.h"

namespace nlive {

namespace audio_renderer {

class CommandBuffer {

private:
  std::vector<QSharedPointer<RenderCommand>> commands_;

public:
  CommandBuffer();
  void addCommand(QSharedPointer<RenderCommand> command);

  const std::vector<QSharedPointer<RenderCommand>>& commands();

};

}

}

#endif