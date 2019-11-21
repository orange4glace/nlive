#ifndef NLIVE_SERVICE_H_
#define NLIVE_SERVICE_H_

#include <string>

namespace nlive {

class IService {

public:
  virtual std::string service_id() const = 0;

};

}

#endif