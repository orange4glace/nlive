#ifndef NLIVE_PROJECT_H_
#define NLIVE_PROJECT_H_

#include <QObject>
#include <QSharedPointer>

#include "platform/undo/undo_stack.h"

namespace nlive {

class StorageDirectory;
class Sequence;

class Project : public QObject {

private:
  QSharedPointer<StorageDirectory> root_storage_directory_;
  sptr<IUndoStack> undo_stack_;

public:
  Project();

  QSharedPointer<StorageDirectory> root_storage_directory();

  QSharedPointer<Sequence> createSequence();

  inline sptr<IUndoStack> Project::undo_stack() { return undo_stack_; }



};

}

#endif