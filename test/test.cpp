#include "gtest/gtest.h"

extern "C" {
#include "/../logging.h"
}

TEST(logging, ok) {
  ASSERT_EQ(form_log(1, 1), 1);
}

TEST(testy, not_ok) {
  ASSERT_EQ(widget_ok(1, 2), 0);
}
