#ifndef NLIVE_PROJECTS_SERVICE_H_
#define NLIVE_PROJECTS_SERVICE_H_

#include <QObject>

namespace nlive {

class IProjectsService  : public QObject {
  Q_OBJECT

protected:
  inline IProjectsService() {}

public:
  virtual void createProject() = 0;

};

}

#endif