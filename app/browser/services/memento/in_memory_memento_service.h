#ifndef NLIVE_IN_MEMORY_MEMENTO_SERVICE_H_
#define NLIVE_IN_MEMORY_MEMENTO_SERVICE_H_

#include <QDebug>
#include "browser/services/memento/memento_service.h"

namespace nlive {

class InMemoryMementoService : public IMementoService {

private:
  MementoScopePtr root_scope_;

public:
  inline InMemoryMementoService() {
    root_scope_ = std::make_shared<MementoScope>("");
  }

  inline MementoScopePtr getScope(std::string path) override {
    return root_scope_->getScope(path);
  }
  
  inline MementoScopePtr getScope(std::initializer_list<std::string> a_args) {
    return root_scope_->getScope(a_args);
  }

  inline MementoScopePtr root_scope() override {
    return root_scope_;
  }
  

};

}

#endif