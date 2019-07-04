#ifndef _NLIVE_VIDEO_RESOUCE_H_
#define _NLIVE_VIDEO_RESOUCE_H_

#include "model/resource/resource.h"

namespace nlive {

class VideoResource : public Resource {
  Q_OBJECT

public:
  VideoResource(QString path);

};

}

#endif