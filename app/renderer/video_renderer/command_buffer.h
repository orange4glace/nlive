#ifndef _NLIVE_VIDEO_RENDERER_COMMAND_BUFFER_H_
#define _NLIVE_VIDEO_RENDERER_COMMAND_BUFFER_H_

#include <vector>

#include "renderer/video_renderer/render_command.h"

namespace nlive {

namespace video_renderer {

class CommandBuffer {

private:
  std::vector<RenderCommand*> commands_;

public:
  CommandBuffer();
  void addCommand(RenderCommand* command);

  const std::vector<RenderCommand*>& commands();

};

}

}

#endif