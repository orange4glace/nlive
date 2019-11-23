#ifndef NLIVE_SERVICE_LOCATOR_H_
#define NLIVE_SERVICE_LOCATOR_H_

#include <map>
#include "base/common/memory.h"
#include "platform/service/service.h"
#include "platform/logger/logger.h"

namespace nlive {

class ServiceLocator {

private:
  std::map<std::string, sptr<IService>> service_map_;

public:
  inline ServiceLocator() {

  }

  template <typename T>
  sptr<T> getService(std::string service_id) {
    if (service_map_.count(service_id) == 0) {
      spdlog::get(LOGGER_DEFAULT)->critical(
        "[ServiceLocator] Can not find service {}!", service_id);
      exit(-1);
    }
    return std::static_pointer_cast<T>(service_map_[service_id]);
  }

  template <typename T>
  void registerService(sptr<T> service) {
    auto service_id = service->service_id();
    if (service_map_.count(service_id) > 0) {
      spdlog::get(LOGGER_DEFAULT)->critical(
        "[ServiceLocator] Duplicate service registration {}!", service_id);
      exit(-1);
    }
    service_map_[service_id] = service;
  }

};

}

#endif