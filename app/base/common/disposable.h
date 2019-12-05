#ifndef NLIVE_DISPOSABLE_H_
#define NLIVE_DISPOSABLE_H_

#include <functional>
#include "base/common/memory.h"

namespace nlive {

class IDisposable {
  
public:
  virtual void dispose() = 0;

};

class Disposable : public IDisposable {

private:
  bool disposed_;
  std::function<void ()> fn_;

public:
  Disposable(std::function<void ()> fn);
  static sptr<Disposable> create(std::function<void ()> fn);

  void dispose() override;

};

}

#endif