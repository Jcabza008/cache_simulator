#include <gtest/gtest.h>

#include <cmath>
#include <queue>
#include <vector>
#include <stdint.h>

#include "scanner.h"
#include "rep_policy.h"
#include "cache.h"

using namespace cache_simulator;

TEST(DirectMapCache, noEntry_Miss)
{
    DirectMapCache cache(8, 32);
    auto memaccess = MemAccessDesc{MemOperation::Load, 0x1234567};
    ASSERT_EQ(cache.accessMem(memaccess), false);
}

TEST(DirectMapCache, several_Addresses)
{
    DirectMapCache cache(8, 32);
    auto memaccess = MemAccessDesc{MemOperation::Load, 0x1234567};
    cache.accessMem(memaccess);
    ASSERT_EQ(cache.accessMem(memaccess), true);
}