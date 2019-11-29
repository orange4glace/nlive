#include "browser/services/projects/projects_service_impl.h"

namespace nlive {

ProjectsService::ProjectsService() :
  target_project_(nullptr) {

}

sptr<Project> ProjectsService::createProject() {
  sptr<Project> project = std::make_shared<Project>();
  projects_.push_back(project);
  onDidAddProject(project);
  return project;
}
  
void ProjectsService::setTargetProject(sptr<Project> project) {
  target_project_ = project;
  onDidChangeTargetProject(project);
}

sptr<Project> ProjectsService::target_project() {
  return target_project_;
}

std::vector<sptr<Project>> ProjectsService::projects() {
  return projects_;
}

}