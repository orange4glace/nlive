#ifndef NLIVE_IMPORT_SERVICE_H_
#define NLIVE_IMPORT_SERVICE_H_

#include <QObject>
#include <QFileInfo>

namespace nlive {

class StorageDirectory;

class IImportService : public QObject {

protected:
  inline IImportService() {}

public:
  virtual void import(QList<QFileInfo> urls, QSharedPointer<StorageDirectory> directory) = 0;

};

}

#endif