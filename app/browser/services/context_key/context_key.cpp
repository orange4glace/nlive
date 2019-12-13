#include "browser/services/context_key/context_key.h"

namespace nlive {

SimpleContextKeyChangeEvent::SimpleContextKeyChangeEvent(std::string _key) :
  key(_key) {}
bool SimpleContextKeyChangeEvent::affectsSome(const std::set<std::string>& keys) {
  return keys.count(key);
}


Context::Context(int id, sptr<Context> parent) :
  id_(id), parent_(parent) {

}

bool Context::setValue(std::string key, sptr<IContextKeyValue> value) {
  auto it = values_.find(key);
  if (it != values_.end()) {
    auto v = it->second;
    if (v->equals(value)) return false;
  }
  values_[key] = value;
  return true;
}

bool Context::removeValue(std::string key) {
  auto it = values_.find(key);
  if (it != values_.end()) {
    values_.erase(it);
    return true;
  }
  return false;
}

sptr<IContextKeyValue> Context::getValue(std::string key) {
  auto it = values_.find(key);
  if (it != values_.end()) {
    return it->second;
  }
  if (parent_) return parent_->getValue(key);
  else return nullptr;
}


ContextKey::ContextKey(sptr<AbstractContextKeyService> service,
  std::string key, sptr<IContextKeyValue> default_value) :
  service_(service), key_(key), default_value_(default_value) {

}

void ContextKey::set(sptr<IContextKeyValue> value) {
  service_->setContext(key_, value);
}

void ContextKey::reset() {
  if (default_value_ == nullptr) service_->removeContext(key_);
  else service_->setContext(key_, default_value_);
}

sptr<IContextKeyValue> ContextKey::get() {
  return service_->getContextKeyValue(key_);
}

AbstractContextKeyService::AbstractContextKeyService(int my_context_id) :
  my_context_id_(my_context_id) {

}

sptr<IContextKey> AbstractContextKeyService::createKey(std::string key, sptr<IContextKeyValue> default_value) {
  return std::make_shared<ContextKey>(shared_from_this(), key, default_value);
}

sptr<IContextKeyValue> AbstractContextKeyService::getContextKeyValue(std::string key) {
  return getContextValuesContainer(my_context_id_)->getValue(key);
}

bool AbstractContextKeyService::contextMatchesRules(ContextKeyExprPtr rules) {
  auto context = getContextValuesContainer(my_context_id_);
  return rules->evaluate(context);
}

void AbstractContextKeyService::setContext(std::string key, sptr<IContextKeyValue> value) {
  auto my_context = getContextValuesContainer(my_context_id_);
  if (!my_context) return;
  if (my_context->setValue(key, value)) {
    SimpleContextKeyChangeEvent e(key);
    onDidChangeContext(&e);
  }
}

void AbstractContextKeyService::removeContext(std::string key) {
  auto my_context = getContextValuesContainer(my_context_id_);
  if (!my_context) return;
  if (my_context->removeValue(key)) {
    SimpleContextKeyChangeEvent e(key);
    onDidChangeContext(&e);
  }
}


ContextKeyService::ContextKeyService() :
  AbstractContextKeyService(0) {
  context_ = std::make_shared<Context>(0, nullptr);
}

sptr<Context> ContextKeyService::getContextValuesContainer(int context_id) {
  return context_;
}


}