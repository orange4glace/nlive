#ifndef NLIVE_EFFECT_CONTROL_LAYOUT_H_
#define NLIVE_EFFECT_CONTROL_LAYOUT_H_

#include <QObject>

namespace nlive {

namespace effect_control {

class EffectControlLayout : public QObject {
  Q_OBJECT

private:
  double side_timeline_ratio_;

public:
  inline EffectControlLayout(double side_timeline_ratio) :
    side_timeline_ratio_(side_timeline_ratio) {

  }

  inline double side_timeline_ratio() const { return side_timeline_ratio_; }

signals:
  void onDidUpdate();

};

}

}

#endif