#ifndef NLIVE_EFFECT_CONTROL_LAYOUT_H_
#define NLIVE_EFFECT_CONTROL_LAYOUT_H_

#include <QObject>

namespace nlive {

namespace effect_control {

class EffectControlLayout : public QObject {
  Q_OBJECT

private:
  int side_width_;
  int timeline_width_;

public:
  void resize(int width, int height);

  inline int side_width() const { return side_width_; }
  inline int timeline_width() const { return timeline_width_; }

signals:
  void onDidUpdate();

};

}

}

#endif