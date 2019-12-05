#include "base/common/disposable.h"

namespace nlive {

Disposable::Disposable(std::function<void ()> fn) :
  disposed_(false), fn_(fn) {

}

sptr<Disposable> Disposable::create(std::function<void ()> fn) {
  return std::make_shared<Disposable>(fn);
}

void Disposable::dispose() {
  if (disposed_) return;
  fn_();
  disposed_ = true;
}

}