#ifndef NLIVE_IWIDGETS_SERVICE_H_
#define NLIVE_IWIDGETS_SERVICE_H_

#include "base/common/sig.h"
#include "platform/service/service.h"
#include "browser/widgets/widget.h"
#include <set>

namespace nlive {

class IWidgetsService : public IService {

protected:
  IWidgetsService() = default;

public:
  virtual void addWidget(Widget* widget) = 0;
  virtual Widget* getWidget(std::string type) = 0;
  virtual std::set<Widget*> getWidgets(std::string type) = 0;

};

}

#endif