#ifndef NLIVE_IMPORT_SERVICE_H_
#define NLIVE_IMPORT_SERVICE_H_

#include <QObject>
#include <QFileInfo>
#include "base/common/memory.h"
#include "platform/service/service.h"

namespace nlive {

class StorageDirectory;

class IImportService : public IService {

protected:
  inline IImportService() {}

public:
  virtual void import(QList<QFileInfo> urls, sptr<StorageDirectory> directory) = 0;

};

}

#endif