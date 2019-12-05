#ifndef NLIVE_CONTEXT_KEY_H_
#define NLIVE_CONTEXT_KEY_H_

#include <string>
#include <vector>
#include <set>
#include <any>
#include "base/common/memory.h"
#include "base/common/sig.h"

namespace nlive {

class IContextKeyService;
class ContextKeyExpr;
typedef sptr<ContextKeyExpr> ContextKeyExprPtr;

class IContext {
public:
  virtual std::any getValue(std::string key) = 0;
};

class IContextKeyExprMapper {

};

enum ContextKeyExprType {
  Defined = 0,
  Not,
  Equals,
  And,
  Or
};

class ContextKeyExpr {

public:
  static ContextKeyExprPtr has(std::string key);
  // static ContextKeyExprPtr equals(std::string key, std::any value);
  static ContextKeyExprPtr not(std::string key);
  static ContextKeyExprPtr and(std::vector<ContextKeyExprPtr> expr);
  static ContextKeyExprPtr or(std::vector<ContextKeyExprPtr> expr);

  ContextKeyExpr() = default;
  virtual ContextKeyExprType getType() const = 0;
  virtual bool equals(ContextKeyExprPtr other) const = 0;
  virtual bool evaluate(sptr<IContext> context) const = 0;
  virtual std::string serialize() const = 0;
  virtual std::vector<std::string> keys() const = 0;
  virtual ContextKeyExprPtr map(sptr<IContextKeyExprMapper> mapFunc) const = 0;
  virtual ContextKeyExprPtr negate() const = 0;
};

#define DECLARE_CONTEXT_KEY_METHODS(ExprType) \
  ContextKeyExprType getType() const override; \
  int cmp(sptr<ExprType> other) const; \
  bool equals(ContextKeyExprPtr other) const override; \
  bool evaluate(sptr<IContext> context) const override; \
  std::string serialize() const override; \
  std::vector<std::string> keys() const override; \
  ContextKeyExprPtr map(sptr<IContextKeyExprMapper> mapFunc) const override; \
  ContextKeyExprPtr negate() const override;

class ContextKeyDefinedExpr : public ContextKeyExpr {

private:
  std::string key_;

  ContextKeyDefinedExpr(std::string key);

public:
  static sptr<ContextKeyDefinedExpr> create(std::string key);

  DECLARE_CONTEXT_KEY_METHODS(ContextKeyDefinedExpr);

};

// class ContextKeyEqualsExpr : public ContextKeyExpr {

// private:
//   std::string key_;
//   std::any value_;

// public:
//   static sptr<ContextKeyExpr> create(std::string key, std::any value);

//   ContextKeyEqualsExpr(std::string key, std::any value);

//   DECLARE_CONTEXT_KEY_METHODS(ContextKeyEqualsExpr);

// };

class ContextKeyNotExpr : public ContextKeyExpr {

private:
  std::string key_;

  ContextKeyNotExpr(std::string key);

public:
  static sptr<ContextKeyExpr> create(std::string key);

  DECLARE_CONTEXT_KEY_METHODS(ContextKeyNotExpr);

};

class ContextKeyAndExpr : public ContextKeyExpr {

private:
  std::vector<ContextKeyExprPtr> expr_;

  ContextKeyAndExpr(std::vector<ContextKeyExprPtr> expr);
  static std::vector<ContextKeyExprPtr> normalizeArr(std::vector<ContextKeyExprPtr> expr);

public:
  static ContextKeyExprPtr create(ContextKeyExprPtr expr[]);
  static ContextKeyExprPtr create(std::vector<ContextKeyExprPtr>&& expr);

  DECLARE_CONTEXT_KEY_METHODS(ContextKeyAndExpr);

};

class ContextKeyOrExpr : public ContextKeyExpr {

private:
  std::vector<ContextKeyExprPtr> expr_;

  ContextKeyOrExpr(std::vector<ContextKeyExprPtr> expr);
  static std::vector<ContextKeyExprPtr> normalizeArr(std::vector<ContextKeyExprPtr> expr);

public:
  static ContextKeyExprPtr create(ContextKeyExprPtr expr[]);
  static ContextKeyExprPtr create(std::vector<ContextKeyExprPtr>&& expr);

  ContextKeyExprType getType() const override;
  bool equals(ContextKeyExprPtr other) const override;
  bool evaluate(sptr<IContext> context) const override;
  std::string serialize() const override;
  std::vector<std::string> keys() const override;
  ContextKeyExprPtr map(sptr<IContextKeyExprMapper> mapFunc) const override;
  ContextKeyExprPtr negate() const override;

};

#undef DECLARE_CONTEXT_KEY_METHODS

class IContextKey {
public:
  virtual void set(std::any value) = 0;
  virtual void reset() = 0;
  virtual std::any get() = 0;
};

struct IContextKeyChangeEvent {
  virtual bool affectsSome(const std::set<std::string>& keys) = 0;
};

class IContextKeyService {
public:
  sptr<IContextKey> createKey(std::string key, std::any default_value);
  sig2_t<void (IContextKeyChangeEvent*)> onDidChangeContext;
};

}


#endif