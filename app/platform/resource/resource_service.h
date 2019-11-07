#ifndef NLIVE_RESOURCE_SERVICE_H_
#define NLIVE_RESOURCE_SERVICE_H_

#include <QSharedPointer>
#include <functional>
#include <QObject>

namespace nlive {

class Resource;

class IResourceService : public QObject {
  Q_OBJECT

protected:
  inline IResourceService() {}

public:
  virtual void loadResource(QString path, std::function<void(QSharedPointer<Resource>)>&& callback) = 0;

};

}

#endif