#ifndef _NLIVE_RESOURCE_SERVICE_H_
#define _NLIVE_RESOURCE_SERVICE_H_

#include <QObject>
#include <uv.h>

namespace nlive {

class Resource;

class IResourceService : public QObject {
  Q_OBJECT

protected:
  IResourceService();

public:
  virtual void loadResource(QString path, void (*callback)(Resource* resource)) = 0;

};

}

#endif