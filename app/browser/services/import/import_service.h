#ifndef _NLIVE_IMPORT_SERVICE_H_
#define _NLIVE_IMPORT_SERVICE_H_

#include <QObject>

namespace nlive {

class StorageDirectory;

class IImportService : public QObject {

protected:
  inline IImportService() {}

public:
  virtual void import(QList<QUrl> urls, QSharedPointer<StorageDirectory> directory) = 0;

};

}

#endif