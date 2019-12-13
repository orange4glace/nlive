#ifndef NLIVE_CONTEXT_KEY_H_
#define NLIVE_CONTEXT_KEY_H_

#include <string>
#include <vector>
#include <set>
#include <any>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "platform/service/service.h"

namespace nlive {

class IContextKeyService;
class ContextKeyExpr;
typedef sptr<ContextKeyExpr> ContextKeyExprPtr;

enum ContextKeyValueType {
  ContextKeyValueTypeBoolean = 0,
  ContextKeyValueTypeDefined,
};

class IContextKeyValue {

public:
  virtual bool defined() = 0;
  virtual bool equals(sptr<IContextKeyValue> value) = 0;
  virtual int compare(sptr<IContextKeyValue> value) = 0;
  virtual std::string serialize() = 0;
  virtual ContextKeyValueType type() = 0;

};

#define DECLARE_CONTEXT_KEY_VALUE_METHODS(ValueType) \
public: \
  bool defined() override; \
  bool equals(sptr<IContextKeyValue> value) override; \
  int compare(sptr<IContextKeyValue> value) override; \
  std::string serialize() override; \
  ContextKeyValueType type() override;

class ContextKeyBooleanValue : public IContextKeyValue {
DECLARE_CONTEXT_KEY_VALUE_METHODS(ContextKeyBooleanValue);

private:
  ContextKeyBooleanValue(bool value);
  bool value_;

public:
  static sptr<IContextKeyValue> create(bool value);

};

class ContextKeyDefinedValue : public IContextKeyValue {
DECLARE_CONTEXT_KEY_VALUE_METHODS(ContextKeyDefinedValue);

private:
  ContextKeyDefinedValue();

public:
  static sptr<IContextKeyValue> create();

};

class ContextKeyStringValue : public IContextKeyValue {
DECLARE_CONTEXT_KEY_VALUE_METHODS(ContextKeyStringValue);

private:
  std::string value_;
  ContextKeyStringValue(std::string value);

public:
  static sptr<IContextKeyValue> create(std::string value);

};

#undef DECLARE_CONTEXT_KEY_VALUE_METHODS



class IContext {
public:
  virtual sptr<IContextKeyValue> getValue(std::string key) = 0;
};

class IContextKeyExprMapper {

};

enum ContextKeyExprType {
  Defined = 0,
  Not,
  Equals,
  NotEquals,
  And,
  Or
};

class ContextKeyExpr {

public:
  static ContextKeyExprPtr has(std::string key);
  static ContextKeyExprPtr equals(std::string key, sptr<IContextKeyValue> value);
  static ContextKeyExprPtr notEquals(std::string key, sptr<IContextKeyValue> value);
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

protected:
  ContextKeyDefinedExpr(std::string key);

public:
  static sptr<ContextKeyDefinedExpr> create(std::string key);
  const std::string& key() const;

  DECLARE_CONTEXT_KEY_METHODS(ContextKeyDefinedExpr);

};

class ContextKeyEqualsExpr : public ContextKeyExpr {

private:
  std::string key_;
  sptr<IContextKeyValue> value_;

public:
  static sptr<ContextKeyExpr> create(std::string key, sptr<IContextKeyValue> value);

  ContextKeyEqualsExpr(std::string key, sptr<IContextKeyValue> value);

  DECLARE_CONTEXT_KEY_METHODS(ContextKeyEqualsExpr);

};

class ContextKeyNotEqualsExpr : public ContextKeyExpr {

private:
  std::string key_;
  sptr<IContextKeyValue> value_;

public:
  static sptr<ContextKeyExpr> create(std::string key, sptr<IContextKeyValue> value);

  ContextKeyNotEqualsExpr(std::string key, sptr<IContextKeyValue> value);

  DECLARE_CONTEXT_KEY_METHODS(ContextKeyNotEqualsExpr);

};

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

class IContextKey {
public:
  virtual void set(sptr<IContextKeyValue> value) = 0;
  virtual void reset() = 0;
  virtual sptr<IContextKeyValue> get() = 0;
};

struct IContextKeyChangeEvent {
  virtual bool affectsSome(const std::set<std::string>& keys) = 0;
};

class IContextKeyService : public IService {
DECLARE_SERVICE("nlive.platform.context_key.ContextKeyService");

public:
  virtual sptr<IContextKey> createKey(std::string key, sptr<IContextKeyValue> default_value) = 0;
  virtual bool contextMatchesRules(ContextKeyExprPtr rules) = 0;
  virtual sptr<IContextKeyValue> getContextKeyValue(std::string key) = 0;

  sig2_t<void (IContextKeyChangeEvent*)> onDidChangeContext;
};

class RawContextKey : public ContextKeyDefinedExpr {

private:
  sptr<IContextKeyValue> default_value_;

public:
  RawContextKey(std::string key, sptr<IContextKeyValue> default_value);
  static sptr<RawContextKey> create(std::string key, sptr<IContextKeyValue> default_value);

  sptr<IContextKey> bindTo(sptr<IContextKeyService> target);
  sptr<IContextKeyValue> getValue(sptr<IContextKeyService> target);
  ContextKeyExprPtr not();
  ContextKeyExprPtr equals(sptr<IContextKeyValue> value);
  ContextKeyExprPtr notEquals(sptr<IContextKeyValue> value);

};

#undef DECLARE_CONTEXT_KEY_METHODS

}


#endif