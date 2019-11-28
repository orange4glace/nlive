#ifndef NLIVE_PROJECTS_SERVICE_IMPL_H_
#define NLIVE_PROJECTS_SERVICE_IMPL_H_

#include "browser/services/projects/projects_service.h"

namespace nlive {

class ProjectsService : public IProjectsService, public Sig {

private:
  std::vector<sptr<Project>> projects_;

public:
  ProjectsService();

  sptr<Project> createProject() override;

  std::vector<sptr<Project>> projects() override;

};

}

#endif