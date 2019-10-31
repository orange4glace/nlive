#ifndef NLIVE_UNDO_COMMAND_H_
#define NLIVE_UNDO_COMMAND_H_

#include <string>
#include "base/common/memory.h"

namespace nlive {

class UndoCommand {

private:
  std::string id_;

protected:
  inline UndoCommand(std::string id) :
    id_(id) {}

  virtual void undo() = 0;
  virtual void redo() = 0;
  inline virtual bool mergeWith(const sptr<UndoCommand> command) { return false; }

  inline const std::string& id() { return id_; }

};

}

#endif