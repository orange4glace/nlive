#include "model/project/project.h"

#include "platform/undo/undo_stack.h"
#include "model/sequence/sequence.h"
#include "model/storage/storage_directory.h"

namespace nlive {

Project::Project() {
  StorageDirectory* root_dir = new StorageDirectory(sharedFromThis(), "root", nullptr);
  root_storage_directory_ = QSharedPointer<StorageDirectory>(root_dir);
  // undo_stack_ = new QUndoStack(this);
}

QSharedPointer<StorageDirectory> Project::root_storage_directory() {
  return root_storage_directory_;
}

}