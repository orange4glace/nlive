#include "browser/services/import/import_service_impl.h"

#include <QList>
#include <QUrl>
#include <QSharedPointer>
#include <QDebug>
#include <QThread>
#include "platform/resource/resource_service.h"
#include "model/storage/storage_directory.h"
#include "model/storage/resource_storage_item.h"
#include "model/storage/video_resource_storage_item.h"
#include "browser/services/import/import_progress_dialog.h"

namespace nlive {

ImportService::ImportService(IResourceService* resource_service) :
  resource_service_(resource_service) {

}

void ImportService::import(QList<QUrl> urls, QSharedPointer<StorageDirectory> directory) {
  auto progress_dialog = new ImportProgressDialog(urls.size());
  progress_dialog->setModal(true);
  progress_dialog->show();

  for (auto& url : urls) {
    resource_service_->loadResource(url.url(), [directory, progress_dialog](QSharedPointer<Resource> resource) {
      auto video_resource = qSharedPointerCast<VideoResource>(resource);
      ResourceStorageItem* item = new VideoResourceStorageItem(directory, nullptr);
      directory->addItem(item);
      progress_dialog->setProgress(progress_dialog->progress() + 1);
    });
  }
}

}