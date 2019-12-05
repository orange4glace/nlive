#include <gtest/gtest.h>
#include "platform/context_key/context_key.h"

using namespace nlive;

TEST(ContextKeyExpr, equals) {
  auto a = ContextKeyExpr::and({
    ContextKeyExpr::has("a1"),
    ContextKeyExpr::has("a2"),
  });
  auto b = ContextKeyExpr::and({
    ContextKeyExpr::has("a2"),
    ContextKeyExpr::has("a1"),
  });
  EXPECT_TRUE(a->equals(b));
}