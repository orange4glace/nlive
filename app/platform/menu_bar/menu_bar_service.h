#ifndef NLIVE_PLATFORM_MENU_BAR_MENU_BAR_SERVICE_H_
#define NLIVE_PLATFORM_MENU_BAR_MENU_BAR_SERVICE_H_

#include <any>
#include <map>
#include <vector>
#include <string>
#include "platform/service/service.h"
#include "platform/menu_bar/menu_bar.h"

namespace nlive {

class IMenuBarService : public IService {

public:
  inline static const std::string ID = "nlive.platform.MenuBarService";

  virtual void updateMenuBar(IMenuBarData menu_data) = 0;

  inline std::string service_id() const override { return ID; }

};

}

#endif