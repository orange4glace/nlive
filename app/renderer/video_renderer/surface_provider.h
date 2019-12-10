#ifndef NLIVE_VIDEO_RENDERER_SURFACE_PROVIDER_H_
#define NLIVE_VIDEO_RENDERER_SURFACE_PROVIDER_H_

#include <QSurface>

namespace nlive {

namespace video_renderer {

class ISurfaceProvider {

public:
  virtual QSurface* createSurface(const QSurfaceFormat& format) = 0;
  virtual void releaseSurface(QSurface* surface) = 0;

};

}

}

#endif