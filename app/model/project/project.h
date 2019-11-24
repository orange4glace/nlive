#ifndef NLIVE_PROJECT_H_
#define NLIVE_PROJECT_H_

#include <QObject>
#include "base/common/memory.h"
#include "base/common/serialize.h"
#include "platform/undo/undo_stack.h"
#include "model/storage/storage_directory.h"

namespace nlive {

class Sequence;

class Project : public std::enable_shared_from_this<Project> {

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & root_storage_directory_;
  }

  sptr<StorageDirectory> root_storage_directory_;
  sptr<IUndoStack> undo_stack_;

public:
  Project();

  sptr<StorageDirectory> root_storage_directory();

  inline sptr<IUndoStack> Project::undo_stack() { return undo_stack_; }



};

}

#endif