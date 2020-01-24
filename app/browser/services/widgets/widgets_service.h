#ifndef NLIVE_IWIDGETS_SERVICE_H_
#define NLIVE_IWIDGETS_SERVICE_H_

#include "base/common/sig.h"
#include "platform/service/service.h"
#include "browser/widgets/widget.h"
#include <set>

namespace nlive {

class IWidgetsService : public IService {

public:
  static const std::string ID;

protected:
  IWidgetsService() = default;

public:
  virtual void addWidget(Widget* widget) = 0;
  virtual Widget* getWidget(std::string type) = 0;
  virtual std::set<Widget*> getWidgets(std::string type) = 0;

  virtual void setFocusedWidget(Widget* widget) = 0;
  virtual Widget* focused_widget() = 0;

  sig2_t<void (Widget* widget)> onDidChangeFocusedWidget;

  inline std::string service_id() const override { return ID; }

};

}

#endif