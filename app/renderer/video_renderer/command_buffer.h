#ifndef _NLIVE_VIDEO_RENDERER_COMMAND_BUFFER_H_
#define _NLIVE_VIDEO_RENDERER_COMMAND_BUFFER_H_

#include <vector>
#include <QSharedPointer>

#include "renderer/video_renderer/render_command.h"

namespace nlive {

namespace video_renderer {

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