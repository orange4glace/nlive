#include "browser/services/import/import_service_impl.h"

#include <QList>
#include <QFileInfo>
#include <QSharedPointer>
#include <QDebug>
#include <QThread>

#include "platform/logger/logger.h"
#include "platform/resource/resource_service.h"
#include "model/storage/storage_directory.h"
#include "model/storage/resource_storage_item.h"
#include "model/storage/video_resource_storage_item.h"
#include "browser/services/import/import_progress_dialog.h"

namespace nlive {

ImportService::ImportService(IResourceService* resource_service) :
  resource_service_(resource_service) {

}

void ImportService::import(QList<QFileInfo> urls, QSharedPointer<StorageDirectory> directory) {
  auto progress_dialog = new ImportProgressDialog(urls.size());
  progress_dialog->setModal(true);
  progress_dialog->show();

  for (auto& url : urls) {
    resource_service_->loadResource(url.absoluteFilePath(), [directory, progress_dialog, url](QSharedPointer<Resource> resource) {
      QSharedPointer<ResourceStorageItem> item = nullptr;
      if (resource == nullptr) {
        spdlog::get(LOGGER_DEFAULT)->warn("[ImportService] Resource is null. file path = {}", url.absoluteFilePath().toStdString());
      }
      else {
        if (resource->type() == VideoResource::TYPE) {
          auto video_resource = qSharedPointerCast<VideoResource>(resource);
          item = QSharedPointer<ResourceStorageItem>(new VideoResourceStorageItem(directory, video_resource));
        }
        spdlog::get(LOGGER_DEFAULT)->warn("[ImportService] No supported type found. type = {}", resource->type());
      }
      if (item) {
        directory->addItem(item);
        progress_dialog->setProgress(progress_dialog->progress() + 1);
      }
    });
  }
}

}