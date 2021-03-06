#include "renderer/audio_renderer/command_buffer.h"

namespace nlive {

namespace audio_renderer {

CommandBuffer::CommandBuffer() {
  
}

void CommandBuffer::addCommand(sptr<RenderCommand> command) {
  commands_.push_back(command);
}

const std::vector<sptr<RenderCommand>>& CommandBuffer::commands() {
  return commands_;
}

}

}