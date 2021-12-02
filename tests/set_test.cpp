#include <gtest/gtest.h>

#include <cmath>
#include <queue>
#include <vector>
#include <stdint.h>

#include "scanner.h"
#include "rep_policy.h"
#include "cache.h"

using namespace cache_simulator;

TEST(SetAssociativeCache, noEntry_Miss_FIFO)
{
    FIFORepPolicy policy(8);
    SetAssociativeCache cache(16, 32, 2, &policy);
    auto memaccess = MemAccessDesc{MemOperation::Load, 0x1234567};
    ASSERT_EQ(cache.accessMem(memaccess), false);
}

TEST(SetAssociativeCache, several_Addresses_FIFO)
{
    FIFORepPolicy policy(8);
    SetAssociativeCache cache(16, 8, 2, &policy);
    std::vector<MemAccessDesc> memaccess = {
        MemAccessDesc{MemOperation::Load, 0x12CF}
    };

    ASSERT_EQ(cache.accessMem(memaccess[0]), false);
    ASSERT_EQ(cache.accessMem(memaccess[0]), true);
}