#include "platform/context_key/context_key.h"

#include <algorithm>
#include <sstream>
#include <deque>
#include <iostream>

namespace nlive {

int cmp(const ContextKeyExprPtr& a, const ContextKeyExprPtr& b) {
  auto at = a->getType();
  auto bt = b->getType();
  if (at != bt) return at - bt;
  switch (at) {
    case ContextKeyExprType::Defined:
      return std::static_pointer_cast<ContextKeyDefinedExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyDefinedExpr>(b));
    case ContextKeyExprType::Not:
      return std::static_pointer_cast<ContextKeyNotExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyNotExpr>(b));
    case ContextKeyExprType::And:
      return std::static_pointer_cast<ContextKeyAndExpr>(a)->cmp(
        std::static_pointer_cast<ContextKeyAndExpr>(b));
  }
  throw "ContextKeyExpr compare exception";
}

struct cmpFunc {
  bool operator()(const ContextKeyExprPtr& a, const ContextKeyExprPtr& b) const {
    return ::nlive::cmp(a, b) > 0;
  }
};

ContextKeyExprPtr ContextKeyExpr::has(std::string key) {
  return ContextKeyDefinedExpr::create(key);
}
// ContextKeyExprPtr ContextKeyExpr::equals(std::string key, std::any value) {}
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
  // TODO
  return false;
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





// ContextKeyEqualsExpr::ContextKeyEqualsExpr(std::string key, std::any value) :
//   key_(key), value_(value) {

// }

// sptr<ContextKeyExpr> ContextKeyEqualsExpr::create(std::string key, std::any value) {
//   if (value.type() == typeid(bool)) {
//     bool v = std::any_cast<bool>(value);
//     if (v) return ContextKeyDefinedExpr::create(key);
//   }
//   return std::make_shared<ContextKeyEqualsExpr>(key, value);
// }

// ContextKeyExprType ContextKeyEqualsExpr::getType() const {
//   return ContextKeyExprType::Equals;
// }

// int ContextKeyEqualsExpr::cmp(sptr<ContextKeyEqualsExpr> other) const {
//   return key_.compare(other->key_);
// }

// bool ContextKeyEqualsExpr::equals(sptr<ContextKeyExpr> other) const {
//   if (other->getType() == ContextKeyExprType::Defined)
//     return key_ == std::static_pointer_cast<ContextKeyEqualsExpr>(other)->key_;
//   return false;
// }

// bool ContextKeyEqualsExpr::evaluate(sptr<IContext> context) const {
//   // TODO
//   return false;
// }

// std::string ContextKeyEqualsExpr::serialize() const {
//   return key_;
// }

// std::vector<std::string> ContextKeyEqualsExpr::keys() const {
//   std::vector<std::string> vec;
//   vec.push_back(key_);
//   return vec;
// }

// ContextKeyExprPtr ContextKeyEqualsExpr::map(sptr<IContextKeyExprMapper> mapFunc) const {
//   // TODO
//   return nullptr;
// }

// ContextKeyExprPtr ContextKeyEqualsExpr::negate() const {
  
// }






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
  if (other->getType() == ContextKeyExprType::Defined)
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






}