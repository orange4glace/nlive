#ifndef NLIVE_MEMENTO_SERVICE_H_
#define NLIVE_MEMENTO_SERVICE_H_

#include "browser/services/memento/memento_scope.h"

namespace nlive {

class IMementoService {

protected:
  inline IMementoService() {}

public:
  virtual MementoScopePtr getScope(std::string path) = 0;
  virtual MementoScopePtr getScope(std::initializer_list<std::string> a_args) = 0;

  virtual MementoScopePtr root_scope() = 0;
  

};

}

#endif