#ifndef NLIVE_BROWSER_PROJECT_WIDGET_ACTIONS_H_
#define NLIVE_BROWSER_PROJECT_WIDGET_ACTIONS_H_

#include "base/common/actions.h"
#include "browser/widgets/project/project_widget.h"

namespace nlive {
  
namespace project_widget {

class GoToParentDirectoryAction : public Action {

public:
  inline static const std::string ID = "nlive.browser.action.ProjectWidget.GoToParentDirectoryAction";
  GoToParentDirectoryAction(ProjectWidget* widget);

  void run(IActionRunParam* param) override;

};

}

}

#endif