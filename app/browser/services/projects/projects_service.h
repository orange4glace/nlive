#ifndef NLIVE_PROJECTS_SERVICE_H_
#define NLIVE_PROJECTS_SERVICE_H_

#include <vector>
#include "base/common/sig.h"
#include "base/common/memory.h"
#include "model/project/project.h"

namespace nlive {

class IProjectsService {

protected:
  inline IProjectsService() {}

public:
  inline virtual sptr<Project> createProject() = 0;
  
  inline virtual std::vector<sptr<Project>> projects();

  sig2_t<void (sptr<Project>)> onDidAddProject;
  sig2_t<void (sptr<Project>)> onDidRemoveProject;

};

}

#endif