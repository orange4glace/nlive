#include "platform/context_key/context_key.h"

#include <algorithm>
#include <sstream>
#include <deque>
#include <iostream>

namespace nlive {

ContextKeyBooleanValue::ContextKeyBooleanValue(bool value) : value_(value) {

}
sptr<IContextKeyValue> ContextKeyBooleanValue::create(bool value) {
  return sptr<IContextKeyValue>(new ContextKeyBooleanValue(value));
}
bool ContextKeyBooleanValue::defined() {
  return value_;
}
bool ContextKeyBooleanValue::equals(sptr<IContextKeyValue> value) {
  if (type() != value->type()) return false;
  auto rhs = std::static_pointer_cast<ContextKeyBooleanValue>(value);
  return value_ == rhs->value_;
}
int ContextKeyBooleanValue::compare(sptr<IContextKeyValue> value) {
  auto at = type();
  auto bt = value->type();
  if (at != bt) return at - bt > 0 ? 1 : -1;
  auto rhs = std::static_pointer_cast<ContextKeyBooleanValue>(value);
  if (value_ == rhs->value_) return 0;
  return value_ ? 1 : -1;
}
std::string ContextKeyBooleanValue::serialize() {
  return value_ ? "true" : "false";
}
ContextKeyValueType ContextKeyBooleanValue::type() {
  return ContextKeyValueTypeBoolean;
}



ContextKeyDefinedValue::ContextKeyDefinedValue() {

}
sptr<IContextKeyValue> ContextKeyDefinedValue::create() {
  return sptr<IContextKeyValue>(new ContextKeyDefinedValue());
}
bool ContextKeyDefinedValue::defined() {
  return true;
}
bool ContextKeyDefinedValue::equals(sptr<IContextKeyValue> value) {
  return (type() == value->type());
}
int ContextKeyDefinedValue::compare(sptr<IContextKeyValue> value) {
  auto at = type();
  auto bt = value->type();
  if (at != bt) return at - bt > 0 ? 1 : -1;
  return 0;
}
std::string ContextKeyDefinedValue::serialize() {
  return "true";
}
ContextKeyValueType ContextKeyDefinedValue::type() {
  return ContextKeyValueTypeDefined;
}


ContextKeyStringValue::ContextKeyStringValue(std::string value) : value_(value) {

}
sptr<IContextKeyValue> ContextKeyStringValue::create(std::string value) {
  return sptr<IContextKeyValue>(new ContextKeyStringValue(value));
}
bool ContextKeyStringValue::defined() {
  return value_ != "";
}
bool ContextKeyStringValue::equals(sptr<IContextKeyValue> value) {
  if (type() != value->type()) return false;
  auto rhs = std::static_pointer_cast<ContextKeyStringValue>(value);
  return value_ == rhs->value_;
}
int ContextKeyStringValue::compare(sptr<IContextKeyValue> value) {
  auto at = type();
  auto bt = value->type();
  if (at != bt) return at - bt > 0 ? 1 : -1;
  auto rhs = std::static_pointer_cast<ContextKeyStringValue>(value);
  return value_.compare(rhs->value_);
}
std::string ContextKeyStringValue::serialize() {
  return value_;
}
ContextKeyValueType ContextKeyStringValue::type() {
  return ContextKeyValueTypeBoolean;
}


int cmp(const ContextKeyExprPtr& a, const ContextKeyExprPtr& b) {
  auto at = a->getType();
  auto bt = b->getType();
  if (at != bt) return at - bt > 0 ? 1 : -1;
  switch (at) {
    case ContextKeyExprType::Defined:
      return std::static_pointer_cast<ContextKeyDefinedExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyDefinedExpr>(b));
    case ContextKeyExprType::Not:
      return std::static_pointer_cast<ContextKeyNotExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyNotExpr>(b));
    case ContextKeyExprType::Equals:
      return std::static_pointer_cast<ContextKeyEqualsExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyEqualsExpr>(b));
    case ContextKeyExprType::NotEquals:
      return std::static_pointer_cast<ContextKeyNotEqualsExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyNotEqualsExpr>(b));
    case ContextKeyExprType::And:
      return std::static_pointer_cast<ContextKeyAndExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyAndExpr>(b));
    default:
      throw "ContextKeyExpr compare exception";
  }
}

struct cmpFunc {
  bool operator()(const ContextKeyExprPtr& a, const ContextKeyExprPtr& b) const {
    return ::nlive::cmp(a, b) > 0;
  }
};

ContextKeyExprPtr ContextKeyExpr::has(std::string key) {
  return ContextKeyDefinedExpr::create(key);
}
ContextKeyExprPtr ContextKeyExpr::equals(std::string key, sptr<IContextKeyValue> value) {
  return ContextKeyEqualsExpr::create(key, value);
}
ContextKeyExprPtr ContextKeyExpr::notEquals(std::string key, sptr<IContextKeyValue> value) {
  return ContextKeyNotEqualsExpr::create(key, value);
}
ContextKeyExprPtr ContextKeyExpr::not(std::string key) {
  return ContextKeyNotExpr::create(key);
}
ContextKeyExprPtr ContextKeyExpr::and(std::vector<ContextKeyExprPtr> expr) {
  return ContextKeyAndExpr::create(std::move(expr));
}
ContextKeyExprPtr ContextKeyExpr::or(std::vector<ContextKeyExprPtr> expr) {
  return ContextKeyOrExpr::create(std::move(expr));
}












ContextKeyDefinedExpr::ContextKeyDefinedExpr(std::string key) :
  key_(key) {

}

sptr<ContextKeyDefinedExpr> ContextKeyDefinedExpr::create(std::string key) {
  return sptr<ContextKeyDefinedExpr>(new ContextKeyDefinedExpr(key));
}

const std::string& ContextKeyDefinedExpr::key() const {
  return key_;
}

ContextKeyExprType ContextKeyDefinedExpr::getType() const {
  return ContextKeyExprType::Defined;
}

int ContextKeyDefinedExpr::cmp(sptr<ContextKeyDefinedExpr> other) const {
  return key_.compare(other->key_);
}

bool ContextKeyDefinedExpr::equals(sptr<ContextKeyExpr> other) const {
  if (other->getType() == ContextKeyExprType::Defined)
    return key_ == std::static_pointer_cast<ContextKeyDefinedExpr>(other)->key_;
  return false;
}

bool ContextKeyDefinedExpr::evaluate(sptr<IContext> context) const {
  auto value = context->getValue(key_);
  if (!value) return false;
  return value->defined();
}

std::string ContextKeyDefinedExpr::serialize() const {
  return key_;
}

std::vector<std::string> ContextKeyDefinedExpr::keys() const {
  std::vector<std::string> vec;
  vec.push_back(key_);
  return vec;
}

ContextKeyExprPtr ContextKeyDefinedExpr::map(sptr<IContextKeyExprMapper> mapFunc) const {
  // TODO
  return nullptr;
}

ContextKeyExprPtr ContextKeyDefinedExpr::negate() const {
  return ContextKeyNotExpr::create(key_);
}





ContextKeyEqualsExpr::ContextKeyEqualsExpr(std::string key, sptr<IContextKeyValue> value) :
  key_(key), value_(value) {

}

sptr<ContextKeyExpr> ContextKeyEqualsExpr::create(std::string key, sptr<IContextKeyValue> value) {
  return sptr<ContextKeyExpr>(new ContextKeyEqualsExpr(key, value));
}

ContextKeyExprType ContextKeyEqualsExpr::getType() const {
  return ContextKeyExprType::Equals;
}

int ContextKeyEqualsExpr::cmp(sptr<ContextKeyEqualsExpr> other) const {
  auto kc = key_.compare(other->key_);
  if (kc != 0) return kc;
  auto vc = value_->compare(other->value_);
  return vc;
}

bool ContextKeyEqualsExpr::equals(sptr<ContextKeyExpr> other) const {
  if (other->getType() == ContextKeyExprType::Equals) {
    auto rhs = std::static_pointer_cast<ContextKeyEqualsExpr>(other);
    return (key_ == rhs->key_) && (value_->compare(rhs->value_) == 0);
  }
  return false;
}

bool ContextKeyEqualsExpr::evaluate(sptr<IContext> context) const {
  auto value = context->getValue(key_);
  if (!value) return false;
  return value->compare(value_) == 0;
}

std::string ContextKeyEqualsExpr::serialize() const {
  return key_ + " == \"" + value_->serialize() + "\"";
}

std::vector<std::string> ContextKeyEqualsExpr::keys() const {
  std::vector<std::string> vec;
  vec.push_back(key_);
  return vec;
}

ContextKeyExprPtr ContextKeyEqualsExpr::map(sptr<IContextKeyExprMapper> mapFunc) const {
  // TODO
  return nullptr;
}

ContextKeyExprPtr ContextKeyEqualsExpr::negate() const {
  return ContextKeyNotEqualsExpr::create(key_, value_);
}





ContextKeyNotEqualsExpr::ContextKeyNotEqualsExpr(std::string key, sptr<IContextKeyValue> value) :
  key_(key), value_(value) {

}

sptr<ContextKeyExpr> ContextKeyNotEqualsExpr::create(std::string key, sptr<IContextKeyValue> value) {
  return sptr<ContextKeyExpr>(new ContextKeyNotEqualsExpr(key, value));
}

ContextKeyExprType ContextKeyNotEqualsExpr::getType() const {
  return ContextKeyExprType::NotEquals;
}

int ContextKeyNotEqualsExpr::cmp(sptr<ContextKeyNotEqualsExpr> other) const {
  auto kc = key_.compare(other->key_);
  if (kc != 0) return kc;
  auto vc = value_->compare(other->value_);
  return vc;
}

bool ContextKeyNotEqualsExpr::equals(sptr<ContextKeyExpr> other) const {
  if (other->getType() == ContextKeyExprType::NotEquals) {
    auto rhs = std::static_pointer_cast<ContextKeyNotEqualsExpr>(other);
    return (key_ == rhs->key_) && (value_->compare(rhs->value_) == 0);
  }
  return false;
}

bool ContextKeyNotEqualsExpr::evaluate(sptr<IContext> context) const {
  auto value = context->getValue(key_);
  if (!value) return !value_->defined();
  return value->compare(value_) != 0;
}

std::string ContextKeyNotEqualsExpr::serialize() const {
  return key_ + " != \"" + value_->serialize() + "\"";
}

std::vector<std::string> ContextKeyNotEqualsExpr::keys() const {
  std::vector<std::string> vec;
  vec.push_back(key_);
  return vec;
}

ContextKeyExprPtr ContextKeyNotEqualsExpr::map(sptr<IContextKeyExprMapper> mapFunc) const {
  // TODO
  return nullptr;
}

ContextKeyExprPtr ContextKeyNotEqualsExpr::negate() const {
  return ContextKeyEqualsExpr::create(key_, value_);
}






ContextKeyNotExpr::ContextKeyNotExpr(std::string key) :
  key_(key) {

}

sptr<ContextKeyExpr> ContextKeyNotExpr::create(std::string key) {
  return sptr<ContextKeyNotExpr>(new ContextKeyNotExpr(key));
}

ContextKeyExprType ContextKeyNotExpr::getType() const {
  return ContextKeyExprType::Not;
}

int ContextKeyNotExpr::cmp(sptr<ContextKeyNotExpr> other) const {
  return key_.compare(other->key_);
}

bool ContextKeyNotExpr::equals(sptr<ContextKeyExpr> other) const {
  if (other->getType() == ContextKeyExprType::Not)
    return key_ == std::static_pointer_cast<ContextKeyNotExpr>(other)->key_;
  return false;
}

bool ContextKeyNotExpr::evaluate(sptr<IContext> context) const {
  // TODO
  return false;
}

std::string ContextKeyNotExpr::serialize() const {
  return "!" + key_;
}

std::vector<std::string> ContextKeyNotExpr::keys() const {
  std::vector<std::string> vec;
  vec.push_back(key_);
  return vec;
}

ContextKeyExprPtr ContextKeyNotExpr::map(sptr<IContextKeyExprMapper> mapFunc) const {
  // TODO
  return nullptr;
}

ContextKeyExprPtr ContextKeyNotExpr::negate() const {
  return ContextKeyDefinedExpr::create(key_);
}






ContextKeyAndExpr::ContextKeyAndExpr(std::vector<ContextKeyExprPtr> expr) :
  expr_(expr) {
}

std::vector<ContextKeyExprPtr> ContextKeyAndExpr::normalizeArr(std::vector<ContextKeyExprPtr> expr) {
  std::vector<ContextKeyExprPtr> arr; 
  for (int i = 0; i < expr.size(); i ++) {
    ContextKeyExprPtr e = expr[i];
    if (e->getType() == ContextKeyExprType::And) {
      auto oand = std::static_pointer_cast<ContextKeyAndExpr>(e);
      arr.insert(arr.end(), oand->expr_.begin(), oand->expr_.end());
      continue;
    }
    if (e->getType() == ContextKeyExprType::Or) {
      throw "It is not allowed to have an or expression here due to lack of parens! For example \"a && (b||c)\" is not supported, use \"(a&&b) || (a&&c)\" instead.";
    }
    arr.push_back(e);
  }
  std::sort(arr.begin(), arr.end(), cmpFunc());
  return arr;
}

ContextKeyExprPtr ContextKeyAndExpr::create(ContextKeyExprPtr expr[]) {
  int n = sizeof(expr) / sizeof(ContextKeyExprPtr);
  std::vector<ContextKeyExprPtr> vec;
  for (int i = 0; i < n; i ++) {
    vec.push_back(expr[i]);
  }
  return sptr<ContextKeyAndExpr>(new ContextKeyAndExpr(normalizeArr(vec)));
}

ContextKeyExprPtr ContextKeyAndExpr::create(std::vector<ContextKeyExprPtr>&& expr) {
  return sptr<ContextKeyAndExpr>(new ContextKeyAndExpr(normalizeArr(expr)));
}

ContextKeyExprType ContextKeyAndExpr::getType() const {
  return ContextKeyExprType::And;
}

int ContextKeyAndExpr::cmp(sptr<ContextKeyAndExpr> other) const {
  if (expr_.size() < other->expr_.size()) return -1;
  if (expr_.size() > other->expr_.size()) return 1;
  for (int i = 0; i < expr_.size(); i ++) {
    auto r = ::nlive::cmp(expr_[i], other->expr_[i]);
    if (r != 0) return r;
  }
  return 0;
}

bool ContextKeyAndExpr::equals(sptr<ContextKeyExpr> other) const {
  if (other->getType() == ContextKeyExprType::And) {
    auto oand = std::static_pointer_cast<ContextKeyAndExpr>(other);
    if (expr_.size() != oand->expr_.size()) {
      return false;
    }
    for (int i = 0; i < expr_.size(); i ++) {
      if (!expr_[i]->equals(oand->expr_[i])) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool ContextKeyAndExpr::evaluate(sptr<IContext> context) const {
  // TODO
  return false;
}

std::string ContextKeyAndExpr::serialize() const {
  std::stringstream ss;
  for (int i = 0; i < expr_.size(); i ++) {
    ss << expr_[i]->serialize();
    if (i != expr_.size() - 1) ss << " && ";
  }
  return ss.str();
}

std::vector<std::string> ContextKeyAndExpr::keys() const {
  std::vector<std::string> vec;
  for (int i = 0; i < expr_.size(); i ++) {
    auto e = expr_[i];
    auto k = e->keys();
    vec.insert(vec.end(), k.begin(), k.end());
  }
  return vec;
}

ContextKeyExprPtr ContextKeyAndExpr::map(sptr<IContextKeyExprMapper> mapFunc) const {
  // TODO
  return nullptr;
}

ContextKeyExprPtr ContextKeyAndExpr::negate() const {
  std::vector<ContextKeyExprPtr> vec;
  for (auto e : expr_) {
    vec.push_back(e->negate());
  }
  return ContextKeyOrExpr::create(std::move(vec));
}






ContextKeyOrExpr::ContextKeyOrExpr(std::vector<ContextKeyExprPtr> expr) :
  expr_(expr) {
}

std::vector<ContextKeyExprPtr> ContextKeyOrExpr::normalizeArr(std::vector<ContextKeyExprPtr> expr) {
  std::vector<ContextKeyExprPtr> arr; 
  for (int i = 0; i < expr.size(); i ++) {
    ContextKeyExprPtr e = expr[i];
    if (e->getType() == ContextKeyExprType::Or) {
      auto oor = std::static_pointer_cast<ContextKeyOrExpr>(e);
      arr.insert(arr.end(), oor->expr_.begin(), oor->expr_.end());
      continue;
    }
    arr.push_back(e);
  }
  std::sort(arr.begin(), arr.end(), cmpFunc());
  return arr;
}

ContextKeyExprPtr ContextKeyOrExpr::create(ContextKeyExprPtr expr[]) {
  int n = sizeof(expr) / sizeof(ContextKeyExprPtr);
  std::vector<ContextKeyExprPtr> vec;
  for (int i = 0; i < n; i ++) {
    vec.push_back(expr[i]);
  }
  return sptr<ContextKeyOrExpr>(new ContextKeyOrExpr(normalizeArr(vec)));
}

ContextKeyExprPtr ContextKeyOrExpr::create(std::vector<ContextKeyExprPtr>&& expr) {
  return sptr<ContextKeyOrExpr>(new ContextKeyOrExpr(normalizeArr(expr)));
}

ContextKeyExprType ContextKeyOrExpr::getType() const {
  return ContextKeyExprType::Or;
}

bool ContextKeyOrExpr::equals(sptr<ContextKeyExpr> other) const {
  if (other->getType() == ContextKeyExprType::And) {
    auto oand = std::static_pointer_cast<ContextKeyOrExpr>(other);
    if (expr_.size() != oand->expr_.size()) {
      return false;
    }
    for (int i = 0; i < expr_.size(); i ++) {
      if (!expr_[i]->equals(oand->expr_[i])) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool ContextKeyOrExpr::evaluate(sptr<IContext> context) const {
  // TODO
  return false;
}

std::string ContextKeyOrExpr::serialize() const {
  std::stringstream ss;
  for (int i = 0; i < expr_.size(); i ++) {
    ss << expr_[i]->serialize();
    if (i != expr_.size() - 1) ss << " || ";
  }
  return ss.str();
}

std::vector<std::string> ContextKeyOrExpr::keys() const {
  std::vector<std::string> vec;
  for (int i = 0; i < expr_.size(); i ++) {
    auto e = expr_[i];
    auto k = e->keys();
    vec.insert(vec.end(), k.begin(), k.end());
  }
  return vec;
}

ContextKeyExprPtr ContextKeyOrExpr::map(sptr<IContextKeyExprMapper> mapFunc) const {
  // TODO
  return nullptr;
}

ContextKeyExprPtr ContextKeyOrExpr::negate() const {
  std::deque<ContextKeyExprPtr> vec;
  for (auto e : expr_) {
    vec.push_back(e->negate());
  }

  auto terminals = [](ContextKeyExprPtr node) {
    if (node->getType() == ContextKeyExprType::Or) {
      auto or = std::static_pointer_cast<ContextKeyOrExpr>(node);
      return or->expr_;
    }
    std::vector<ContextKeyExprPtr> vec;
    vec.push_back(node);
    return vec;
  };

  while (vec.size() > 1) {
    auto LEFT = vec.front();
    vec.pop_front();
    auto RIGHT = vec.front();
    vec.pop_front();
    std::vector<ContextKeyExprPtr> all;
    for (auto left : terminals(LEFT)) {
      for (auto right : terminals(RIGHT)) {
        all.push_back(ContextKeyExpr::and({left, right}));
      }
    }
    for (int i = all.size() - 1; i >= 0; i --) {
      vec.push_front(all[i]);
    }
  }
  return vec[0];
}




RawContextKey::RawContextKey(std::string key, sptr<IContextKeyValue> default_key) :
  ContextKeyDefinedExpr(key), default_value_(default_key) {

}

sptr<RawContextKey> RawContextKey::create(std::string key, sptr<IContextKeyValue> default_value) {
  return std::make_shared<RawContextKey>(key, default_value);
}

sptr<IContextKey> RawContextKey::bindTo(sptr<IContextKeyService> target) {
  return target->createKey(key(), default_value_);
}

sptr<IContextKeyValue> RawContextKey::getValue(sptr<IContextKeyService> target) {
  return target->getContextKeyValue(key());
}

ContextKeyExprPtr RawContextKey::not() {
  return ContextKeyExpr::not(key());
}

ContextKeyExprPtr RawContextKey::equals(sptr<IContextKeyValue> value) {
  return ContextKeyExpr::equals(key(), value);
}

ContextKeyExprPtr RawContextKey::notEquals(sptr<IContextKeyValue> value) {
  return ContextKeyExpr::notEquals(key(), value);
}


}