#ifndef _NLIVE_IMPORT_SERVICE_IMPL_H_
#define _NLIVE_IMPORT_SERVICE_IMPL_H_

#include <QObject>
#include "browser/services/import/import_service.h"

namespace nlive {

class IResourceService;
class ImportProgressDialog;

class ImportService : public IImportService {

private:
  IResourceService* resource_service_;

  QList<QUrl> current_urls_;

public:
  ImportService(IResourceService* resource_service);

  void import(QList<QUrl> urls, QSharedPointer<StorageDirectory> directory) override;

};

}

#endif