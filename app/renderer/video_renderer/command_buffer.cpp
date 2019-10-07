#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace video_renderer {

CommandBuffer::CommandBuffer() {
  
}

void CommandBuffer::addCommand(RenderCommand* command) {
  commands_.push_back(command);
}

const std::vector<RenderCommand*>& CommandBuffer::commands() {
  return commands_;
}

}

}