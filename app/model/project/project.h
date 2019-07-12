#ifndef _NLIVE_PROJECT_H_
#define _NLIVE_PROJECT_H_

#include <QObject>
#include <QUndoStack>
#include <QSharedPointer>

namespace nlive {

class StorageDirectory;
class Sequence;

class Project : public QObject {

private:
  QSharedPointer<StorageDirectory> root_storage_directory_;
  QUndoStack* undo_stack_;

public:
  Project();

  QSharedPointer<StorageDirectory> root_storage_directory();

  QSharedPointer<Sequence> createSequence();

  QUndoStack Project::undo_stack();



};

}

#endif