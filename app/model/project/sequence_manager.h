#ifndef _NLIVE_SEQUENCE_MANAGER_H_
#define _NLIVE_SEQUENCE_MANAGER_H_

#include <set>

namespace nlive {

class Sequence;

class SequenceManager {

private:
  std::set<Sequence*> sequences_;

public:
  SequenceManager();

  Sequence* createSequence();
  void addSequence();

};

}

#endif