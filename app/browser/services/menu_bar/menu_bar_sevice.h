#ifndef NLIVE_MENU_BAR_SERVICE_H_
#define NLIVE_MENU_BAR_SERVICE_H_

#include <QAction>

namespace nlive {

class IMenu {



};

class IMenuBarService {

public:
  virtual QAction* addAction(QAction* action) = 0;

};

}

#endif