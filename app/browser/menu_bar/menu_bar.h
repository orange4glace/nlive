#ifndef NLIVE_MENU_BAR_H_
#define NLIVE_MENU_BAR_H_

#include <QSharedPointer>
#include "platform/service/service_locator.h"

namespace nlive {

class MenuBar {

private:
  QSharedPointer<ServiceLocator> service_locator_;

public:
  MenuBar(QSharedPointer<ServiceLocator> service_locator);

};

}

#endif