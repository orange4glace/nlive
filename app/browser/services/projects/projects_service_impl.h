#ifndef NLIVE_PROJECTS_SERVICE_IMPL_H_
#define NLIVE_PROJECTS_SERVICE_IMPL_H_

#include "browser/services/projects/projects_service.h"

namespace nlive {

class ProjectsService : public IProjectsService, public Sig {

private:
  sptr<Project> target_project_;
  std::vector<sptr<Project>> projects_;

public:
  ProjectsService();

  sptr<Project> createProject() override;
  
  void setTargetProject(sptr<Project> project) override;
  sptr<Project> target_project() override;

  std::vector<sptr<Project>> projects() override;

};

}

#endif