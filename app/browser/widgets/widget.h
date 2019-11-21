#ifndef NLIVE_WIDGET_H_
#define NLIVE_WIDGET_H_

#include <string>

namespace nlive {

class Widget {

public:
  virtual std::string name() const = 0;

};

}

#endif 