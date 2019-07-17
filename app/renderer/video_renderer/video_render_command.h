#ifndef _NLIVE_VIDEO_RENDER_COMMAND_H_
#define _NLIVE_VIDEO_RENDER_COMMAND_H_

#include <stdint.h>

#include "renderer/video_renderer/render_command.h"

namespace nlive {

namespace video_renderer {

class VideoRenderCommand : public RenderCommand {

private:
  int video_resource_id_;
  int64_t timestamp_;

public:
  VideoRenderCommand(int video_resource_id, int64_t timestamp);

};

}

}

#endif