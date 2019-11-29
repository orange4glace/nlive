#ifndef NLIVE_PROJECTS_SERVICE_H_
#define NLIVE_PROJECTS_SERVICE_H_

#include <vector>
#include "platform/service/service.h"
#include "base/common/sig.h"
#include "base/common/memory.h"
#include "model/project/project.h"

namespace nlive {

class IProjectsService : public IService {

protected:
  inline IProjectsService() {}

public:
  static const std::string ID;

  inline virtual sptr<Project> createProject() = 0;

  inline virtual std::vector<sptr<Project>> projects() = 0;

  inline std::string service_id() const {
    return ID;
  }

  sig2_t<void (sptr<Project>)> onDidAddProject;
  sig2_t<void (sptr<Project>)> onWillRemoveProject;

};

}

#endif