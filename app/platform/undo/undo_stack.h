#ifndef NLIVE_UNDO_STACK_H_
#define NLIVE_UNDO_STACK_H_

#include "base/common/memory.h"
#include "platform/undo/undo_command.h"

namespace nlive {

class IUndoStack {

public:
  virtual bool canUndo() = 0;
  virtual bool canRedo() = 0;

  virtual void undo() = 0;
  virtual void redo() = 0;
  virtual void push(sptr<UndoCommand> cmd) = 0;

  virtual void clear() = 0;

};

}

#endif