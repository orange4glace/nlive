#ifndef NLIVE_WIDGETS_SERVICE_H_
#define NLIVE_WIDGETS_SERVICE_H_

#include <map>
#include <set>
#include "browser/services/widgets/widgets_service.h"
#include "browser/main_window.h"

namespace nlive {

class WidgetsService : public IWidgetsService {

private:
  std::map<std::string, std::set<Widget*>> widgets_;

  MainWindow* main_window_;

public:
  WidgetsService();

  void addWidget(Widget* widget) override;
  Widget* getWidget(std::string type) override;
  std::set<Widget*> getWidgets(std::string type) override;

  void setMainWindow(MainWindow* main_window);

  inline std::string service_id() const override { return "service.WidgetsService"; }

};

}

#endif