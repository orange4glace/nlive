#include <gtest/gtest.h>
#include "platform/context_key/context_key.h"

using namespace nlive;

TEST(ContextKeyExpr, equals) {
  auto a = ContextKeyExpr::and({
    ContextKeyExpr::has("a1"),
    ContextKeyExpr::has("a2"),
    ContextKeyExpr::notEquals("c1", ContextKeyBooleanValue::create(true)),
    ContextKeyExpr::notEquals("c2", ContextKeyStringValue::create("cc2")),
    ContextKeyExpr::not("d1"),
    ContextKeyExpr::not("d2"),
  });
  auto b = ContextKeyExpr::and({
    ContextKeyExpr::not("d1"),
    ContextKeyExpr::has("a2"),
    ContextKeyExpr::notEquals("c1", ContextKeyBooleanValue::create(true)),
    ContextKeyExpr::has("a1"),
    ContextKeyExpr::notEquals("c2", ContextKeyStringValue::create("cc2")),
    ContextKeyExpr::not("d2"),
  });
  EXPECT_TRUE(a->equals(b));
}