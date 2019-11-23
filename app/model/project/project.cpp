#include "model/project/project.h"

#include "platform/undo/undo_stack.h"
#include "model/sequence/sequence.h"
#include "model/storage/storage_directory.h"

namespace nlive {

Project::Project() {
  // https://forum.libcinder.org/topic/solution-calling-shared-from-this-in-the-constructor
  auto wptr = std::shared_ptr<Project>( this, [](Project*){} );
  StorageDirectory* root_dir = new StorageDirectory(shared_from_this(), "root", nullptr);
  root_storage_directory_ = sptr<StorageDirectory>(root_dir);
  // undo_stack_ = new QUndoStack(this);
}

sptr<StorageDirectory> Project::root_storage_directory() {
  return root_storage_directory_;
}

}