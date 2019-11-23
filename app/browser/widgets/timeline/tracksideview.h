#ifndef NLIVE_TIMELINE_WIDGET_TRACK_SIDE_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_TRACK_SIDE_VIEW_H_

#include <QWidget>
#include <QLabel>
#include "base/common/memory.h"
#include <QLayout>

namespace nlive {

class Track;
class IThemeService;

namespace timeline_widget {

class TrackSideView : public QWidget {

private:
  sptr<IThemeService> theme_service_;

  sptr<Track> track_;

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  TrackSideView(QWidget* parent, sptr<Track> track, sptr<IThemeService> theme_service);
  

};

}

}

#endif