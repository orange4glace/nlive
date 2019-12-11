#ifndef NLIVE_SERVICE_H_
#define NLIVE_SERVICE_H_

#include <string>

namespace nlive {

#define DECLARE_SERVICE(SERVICE_ID) \
  public: \
    inline static const std::string ID = SERVICE_ID; \
    inline std::string service_id() const override { return ID; }

class IService {

public:
  virtual std::string service_id() const = 0;

};

}

#endif