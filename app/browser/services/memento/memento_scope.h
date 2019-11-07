#ifndef NLIVE_MEMENTO_SCOPE_H_
#define NLIVE_MEMENTO_SCOPE_H_

#include <string>
#include <cassert>
#include <memory>
#include <map>
#include <initializer_list>
#include <QDebug>
#include <iostream>

namespace nlive {

class MementoScope;

typedef std::shared_ptr<MementoScope> MementoScopePtr;

class MementoScope : public std::enable_shared_from_this<MementoScope> {

private:
  void* value_;

  std::map<std::string, MementoScopePtr> children_;

  inline MementoScopePtr doGetScope(std::initializer_list<std::string> const& a_args, int index) {
    if (index >= a_args.size()) return shared_from_this();
    auto child = getOrCreateChildScope(*(a_args.begin() + index));
    return child->doGetScope(a_args, index + 1);
  }

public:
  inline MementoScope(std::string name) {
    value_ = nullptr;
  }

  MementoScopePtr getOrCreateChildScope(std::string name) {
    if (children_.count(name)) return children_[name];
    MementoScopePtr cptr = std::make_shared<MementoScope>(name);
    children_[name] = cptr;
    return cptr;
  }

  inline MementoScopePtr getChildScope(std::string name) {
    assert(children_.count(name));
    return children_[name];
  }

  inline MementoScopePtr getScope(std::string path) {
    if (path.size() == 0) return shared_from_this();
    for (int i = 0; i < path.size(); i ++) {
      if (path[i] == '/') {
        std::string child_name = path.substr(0, i);
        auto child = getOrCreateChildScope(child_name);
        return child->getScope(path.substr(i + 1, path.size() - i));
      }
    }
    return getChildScope(path);
  }

  inline MementoScopePtr getScope(std::initializer_list<std::string> a_args) {
    return doGetScope(a_args, 0);
  }

  bool hasValue() {
    return value_ != nullptr;
  }

template<typename T>
  void setValue(T value) {
    value_ = new T(value);
  }

template<typename T>
  T getValue() {
    assert(value_ != nullptr);
    return *(static_cast<T*>(value_));
  }  

};

}

#endif