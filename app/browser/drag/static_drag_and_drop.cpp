#include "browser/drag/static_drag_and_drop.h"

namespace nlive {

QDrag* StaticDragAndDrop::drag_ = nullptr;
void* StaticDragAndDrop::data_ = nullptr;

void StaticDragAndDrop::setData(QDrag* drag, void* data) {
  StaticDragAndDrop::drag_ = drag;
  StaticDragAndDrop::data_ = data;
}

QDrag* StaticDragAndDrop::drag() {
  return drag_;
}

void* StaticDragAndDrop::data() {
  return data_;
}

}