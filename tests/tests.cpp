#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <queue>
#include <sstream>
#include <cmath>
#include <stdint.h>

#include "scanner.h"
#include "rep_policy.h"
#include "cache.h"

#include "scanner_test.cpp"
#include "rep_policy_test.cpp"
#include "direct_test.cpp"
#include "fully_test.cpp"
#include "set_test.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}