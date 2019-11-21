#include "browser/widgets_service.h"

#include <assert.h>

namespace nlive {

WidgetsService::WidgetsService() {

}

void WidgetsService::addWidget(Widget* widget) {
  auto& set = widgets_[widget->name()];
  set.insert(widget);
}

Widget* WidgetsService::getWidget(std::string type) {
  if (!widgets_.count(type)) return nullptr;
  auto& set = widgets_[type];
  if (set.size() == 0) return nullptr;
  return *set.begin();
}

std::set<Widget*> WidgetsService::getWidgets(std::string type) {
  if (!widgets_.count(type)) return std::set<Widget*>();
  return widgets_[type];
}

void WidgetsService::setMainWindow(MainWindow* main_window) {
  main_window_ = main_window;
}



}