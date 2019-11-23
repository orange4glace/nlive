#ifndef NLIVE_MENU_BAR_H_
#define NLIVE_MENU_BAR_H_

#include "base/common/memory.h"
#include "platform/service/service_locator.h"

namespace nlive {

class MenuBar {

private:
  sptr<ServiceLocator> service_locator_;

public:
  MenuBar(sptr<ServiceLocator> service_locator);

};

}

#endif