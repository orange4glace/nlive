#ifndef NLIVE_TIMELINE_WIDGET_TRACK_SIDE_VIEW_H_
#define NLIVE_TIMELINE_WIDGET_TRACK_SIDE_VIEW_H_

#include <QWidget>
#include <QLabel>
#include <QSharedPointer>
#include <QLayout>

namespace nlive {

class Track;
class IThemeService;

namespace timelinewidget {

class TrackSideView : public QWidget {

private:
  QSharedPointer<IThemeService> theme_service_;

  QSharedPointer<Track> track_;

protected:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

public:
  TrackSideView(QWidget* parent, QSharedPointer<Track> track, QSharedPointer<IThemeService> theme_service);
  

};

}

}

#endif