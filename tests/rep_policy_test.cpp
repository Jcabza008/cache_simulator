#include <vector>
#include <queue>
#include <sstream>
#include <cmath>
#include <stdint.h>

#include "scanner.h"
#include "cache.h"
#include "rep_policy.h"

using namespace cache_simulator;

TEST(FIFORepPolicy, OneSet)
{
    FIFORepPolicy policy(1);
    policy.stored(0, 5);
    policy.stored(0, 3);
    ASSERT_EQ(policy.replace(0), 5);
    ASSERT_EQ(policy.replace(0), 3);
}

TEST(FIFORepPolicy, TwoSets)
{
    FIFORepPolicy policy(2);
    policy.stored(0, 5);
    policy.stored(1, 3);
    policy.stored(0, 2);
    policy.stored(1, 1);
    ASSERT_EQ(policy.replace(0), 5);
    ASSERT_EQ(policy.replace(0), 2);
    ASSERT_EQ(policy.replace(1), 3);
    ASSERT_EQ(policy.replace(1), 1);
}

TEST(LRURepPolicy, OneSet)
{
    LRURepPolicy policy(1, 8);
    policy.stored(0, 5);
    policy.stored(0, 3);
    policy.stored(0, 5);
    ASSERT_EQ(policy.replace(0), 3);
    ASSERT_EQ(policy.replace(0), 5);
}

TEST(LRURepPolicy, TwoSets)
{
    LRURepPolicy policy(2, 8);
    policy.stored(0, 5);
    policy.stored(1, 3);
    policy.stored(0, 2);
    policy.stored(1, 1);

    policy.stored(0, 5);

    ASSERT_EQ(policy.replace(0), 2);
    ASSERT_EQ(policy.replace(0), 5);
    ASSERT_EQ(policy.replace(1), 3);
    ASSERT_EQ(policy.replace(1), 1);
}