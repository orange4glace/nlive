#ifndef NLIVE_AUDIO_RENDERER_COMMAND_BUFFER_H_
#define NLIVE_AUDIO_RENDERER_COMMAND_BUFFER_H_

#include <vector>
#include "base/common/memory.h"

#include "renderer/audio_renderer/render_command.h"

namespace nlive {

namespace audio_renderer {

class CommandBuffer {

private:
  std::vector<sptr<RenderCommand>> commands_;

public:
  CommandBuffer();
  void addCommand(sptr<RenderCommand> command);

  const std::vector<sptr<RenderCommand>>& commands();

};

}

}

#endif