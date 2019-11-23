#ifndef NLIVE_MONITOR_WIDET_SEQUENCE_GIZMO_VIEW_H_
#define NLIVE_MONITOR_WIDET_SEQUENCE_GIZMO_VIEW_H_

#include "base/common/memory.h"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QThread>

#include "renderer/video_renderer/sequence_renderer.h"

namespace nlive {

class Sequence;

namespace monitor_widget {

class SequenceGizmoView : public QWidget {

private:
  sptr<Sequence> sequence_;
  video_renderer::SequenceRenderer* sequence_renderer_;

  void scheduleRender();

protected:

public:
  SequenceGizmoView(QWidget* parent, sptr<Sequence> sequence);

};

}

}

#endif