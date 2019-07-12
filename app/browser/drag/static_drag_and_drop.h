#ifndef _NLIVE_STATIC_DRAG_AND_DROP_H_
#define _NLIVE_STATIC_DRAG_AND_DROP_H_

#include <QDrag>

namespace nlive {

class StaticDragAndDrop {

private:
  static QDrag* drag_;
  static void* data_;

public:
  static void setData(QDrag* drag, void* data);

  static QDrag* drag();
  static void* data();

};

}

#endif