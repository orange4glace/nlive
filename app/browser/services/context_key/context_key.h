#ifndef NLIVE_BROWSER_CONTEXT_KEY_H_
#define NLIVE_BROWSER_CONTEXT_KEY_H_

#include <map>
#include <memory>
#include "base/common/sig.h"
#include "platform/context_key/context_key.h"

namespace nlive {

struct SimpleContextKeyChangeEvent : IContextKeyChangeEvent {
  const std::string key;
  SimpleContextKeyChangeEvent(std::string key);
  bool affectsSome(const std::set<std::string>& keys) override;
};


class Context : public IContext {

private:
  int id_;
  sptr<Context> parent_;
  std::map<std::string, sptr<IContextKeyValue>> values_;

public:
  Context(int id, sptr<Context> parent);
  bool setValue(std::string key, sptr<IContextKeyValue> value);
  bool removeValue(std::string key);
  sptr<IContextKeyValue> getValue(std::string key) override;

};

class AbstractContextKeyService : public IContextKeyService, public Sig,
    public std::enable_shared_from_this<AbstractContextKeyService> {

private:
  int my_context_id_;

public:
  AbstractContextKeyService(int my_context_id);
  
  sptr<IContextKey> createKey(std::string key, sptr<IContextKeyValue> default_value) override;
  sptr<IContextKeyValue> getContextKeyValue(std::string key) override;
  bool contextMatchesRules(ContextKeyExprPtr rules) override;
  void setContext(std::string key, sptr<IContextKeyValue> value);
  void removeContext(std::string key);
  virtual sptr<Context> getContextValuesContainer(int context_id) = 0;

  sig2_t<void (IContextKeyChangeEvent*)> onDidChangeContext;

};

class ContextKeyService : public AbstractContextKeyService {

private:
  sptr<Context> context_;

public:
  ContextKeyService();

  sptr<Context> getContextValuesContainer(int context_id) override;
};

class ContextKey : public IContextKey {

private:
  sptr<AbstractContextKeyService> service_;
  std::string key_;
  sptr<IContextKeyValue> default_value_;

public:
  ContextKey(sptr<AbstractContextKeyService> service,
    std::string key, sptr<IContextKeyValue> default_value);

  void set(sptr<IContextKeyValue> value) override;
  void reset() override;
  sptr<IContextKeyValue> get() override;

};

}

#endif