#ifndef NLIVE_IMPORT_SERVICE_IMPL_H_
#define NLIVE_IMPORT_SERVICE_IMPL_H_

#include <QObject>
#include "browser/services/import/import_service.h"

#include "model/storage/storage_item.h"

namespace nlive {

class IResourceService;
class ImportProgressDialog;

class ImportService : public IImportService {

private:
  IResourceService* resource_service_;
  QList<QFileInfo> current_urls_;

  sptr<StorageItem> doImportVideoContainer(QFileInfo& url, sptr<StorageDirectory> directory);
  sptr<StorageItem> doImportAudio(QFileInfo& url, sptr<StorageDirectory> directory);

public:
  ImportService(IResourceService* resource_service);

  void import(QList<QFileInfo> urls, sptr<StorageDirectory> directory) override;

  inline std::string service_id() const override { return "service.ImportService"; }

};

}

#endif