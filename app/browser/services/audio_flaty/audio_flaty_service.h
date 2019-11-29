#ifndef NLIVE_AUDIO_FLATY_SERVICE_H_
#define NLIVE_AUDIO_FLATY_SERVICE_H_

#include <map>
#include <vector>
#include "platform/service/service.h"
#include "browser/services/projects/projects_service.h"
#include "platform/task/task_service.h"
#include "model/resource/audio_resource.h"
#include "model/storage/storage_directory.h"

namespace nlive {

namespace {

class Worker {

private:
  sptr<ITaskService> task_service_;
  std::map<std::string, std::vector<sptr<AudioResource>>> processing_map_;
  std::map<std::string, sptr<RawAudioResource>> processed_map_;

public:
  Worker(sptr<ITaskService> task_service);

  void addWork(sptr<AudioResource> audio_resource);
  void removeWork(sptr<AudioResource> audio_resource);

};

class DirectoryObserver : public Sig {

private:
  sptr<StorageDirectory> directory_;
  std::map<sptr<StorageDirectory>, sptr<DirectoryObserver>> child_observers_;
  sptr<Worker> worker_;

public:
  DirectoryObserver(sptr<StorageDirectory> directory, sptr<Worker> worker);

};

class ProjectObserver : public Sig {

private:
  sptr<Project> project_;
  sptr<DirectoryObserver> root_observer_;
  sptr<Worker> worker_;

public:
  ProjectObserver(sptr<Project> project, sptr<Worker> worker);

};

}

class AudioFlatyService : public IService, public Sig {

private:
  sptr<Worker> worker_;
  std::map<sptr<Project>, sptr<ProjectObserver>> project_observers_;

public:
  const static std::string ID;

  AudioFlatyService(
    sptr<IProjectsService> projects_service,
    sptr<ITaskService> task_service);

  inline std::string service_id() const {
    return ID;
  }

};

}

#endif