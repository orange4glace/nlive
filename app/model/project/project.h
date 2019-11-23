#ifndef NLIVE_PROJECT_H_
#define NLIVE_PROJECT_H_

#include <QObject>
#include <QSharedPointer>
#include <QEnableSharedFromThis>
#include <boost/serialization/serialization.hpp>
#include "platform/undo/undo_stack.h"
#include "model/storage/storage_directory.h"

namespace nlive {

class Sequence;

class Project : public QEnableSharedFromThis<Project> {

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {

  }

  QSharedPointer<StorageDirectory> root_storage_directory_;
  sptr<IUndoStack> undo_stack_;

public:
  Project();

  QSharedPointer<StorageDirectory> root_storage_directory();

  inline sptr<IUndoStack> Project::undo_stack() { return undo_stack_; }



};

}

#endif