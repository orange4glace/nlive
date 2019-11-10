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
#include "model/storage/audio_storage_item.h"
#include "browser/services/import/import_progress_dialog.h"

namespace nlive {

ImportService::ImportService(IResourceService* resource_service) :
  resource_service_(resource_service) {

}

QSharedPointer<StorageItem> ImportService::doImportVideoContainer(QFileInfo& url, QSharedPointer<StorageDirectory> directory) {
  auto video_resource = resource_service_->loadBestVideoResource(url.absoluteFilePath());
  if (video_resource == nullptr) {
    spdlog::get(LOGGER_DEFAULT)->warn("[ImportService] Failed to import VideoContainer. VideoResource is null. file path = {}", url.absoluteFilePath().toStdString());
    return nullptr;
  }
  auto audio_resource = resource_service_->loadBestAudioResource(url.absoluteFilePath());
  return QSharedPointer<VideoResourceStorageItem>(
    new VideoResourceStorageItem(directory, url.fileName(), video_resource, audio_resource));
}

QSharedPointer<StorageItem> ImportService::doImportAudio(QFileInfo& url, QSharedPointer<StorageDirectory> directory) {
  auto audio_resource = resource_service_->loadBestAudioResource(url.absoluteFilePath());
  if (audio_resource == nullptr) {
    spdlog::get(LOGGER_DEFAULT)->warn("[ImportService] Failed to import Audio. AudioResource is null. file path = {}", url.absoluteFilePath().toStdString());
    return nullptr;
  }
  return QSharedPointer<AudioStorageItem>(
    new AudioStorageItem(directory, url.fileName(), audio_resource));
}

void ImportService::import(QList<QFileInfo> urls, QSharedPointer<StorageDirectory> directory) {
  auto progress_dialog = new ImportProgressDialog(urls.size());
  progress_dialog->setModal(true);
  progress_dialog->show();

  for (auto& url : urls) {
    QString extension = url.suffix();
    QSharedPointer<StorageItem> item = nullptr;
    if (extension == "mp4") {
      item = doImportVideoContainer(url, directory);
    }
    else if (extension == "mp3") {
      item = doImportAudio(url, directory);
    }

    if (item) {
      directory->addItem(item);
      progress_dialog->setProgress(progress_dialog->progress() + 1);
    }
  }
}

}