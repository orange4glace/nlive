#include "browser/services/projects/projects_service_impl.h"

namespace nlive {

ProjectsService::ProjectsService() {

}

sptr<Project> ProjectsService::createProject() {
  sptr<Project> project = std::make_shared<Project>();
  projects_.push_back(project);
  onDidAddProject(project);
  return project;
}

std::vector<sptr<Project>> ProjectsService::projects() {
  return projects_;
}

}