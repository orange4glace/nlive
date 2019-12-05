#ifndef NLIVE_BROWSER_CONTEXT_KEY_H_
#define NLIVE_BROWSER_CONTEXT_KEY_H_

#include <map>
#include "platform/context_key/context_key.h"

namespace nlive {

class Context : public IContext {

private:
  sptr<Context> parent_;
  std::map<std::string, std::any> value_;
  int id_;

public:
  Context(int id, sptr<Context> parent);
  bool setValue(std::string key, std::any value);
  bool removeValue(std::string key);
  std::any getValue(std::string key) override;

};

class AbstractContextKeyService : public IContextKeyService {

public:
  sptr<IContextKey> createKey(std::string key, std::any default_value);
  std::any getContextKeyValue(std::string key);
  void setContext(std::string key, std::any value);
  void removeContext(std::string key);

};

class ContextKey : public IContextKey {

private:


};

}

#endif