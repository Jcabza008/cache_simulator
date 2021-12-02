#include <gtest/gtest.h>

#include <cmath>
#include <queue>
#include <vector>
#include <stdint.h>

#include "scanner.h"
#include "rep_policy.h"
#include "cache.h"

using namespace cache_simulator;

TEST(FullyAssociativeCache, noEntry_Miss_FIFO)
{
    FIFORepPolicy policy(1);
    FullyAssociativeCache cache(8, 32, &policy);
    auto memaccess = MemAccessDesc{MemOperation::Load, 0x1234567};
    ASSERT_EQ(cache.accessMem(memaccess), false);
}

TEST(FullyAssociativeCache, several_Addresses_FIFO)
{
    FIFORepPolicy policy(1);
    FullyAssociativeCache cache(8, 32, &policy);
    std::vector<MemAccessDesc> memaccess = {
        MemAccessDesc{MemOperation::Load, 0x1000000},
        MemAccessDesc{MemOperation::Load, 0x2000000},
        MemAccessDesc{MemOperation::Load, 0x3000000},
        MemAccessDesc{MemOperation::Load, 0x4000000},
        MemAccessDesc{MemOperation::Load, 0x5000000},
        MemAccessDesc{MemOperation::Load, 0x6000000},
        MemAccessDesc{MemOperation::Load, 0x7000000},
        MemAccessDesc{MemOperation::Load, 0x8000000},
        MemAccessDesc{MemOperation::Load, 0x9000000},
        MemAccessDesc{MemOperation::Load, 0xA000000}
    };

    for(auto i = 0; i < 8; i++)
        ASSERT_EQ(cache.accessMem(memaccess[i]), false);

    ASSERT_EQ(cache.accessMem(memaccess[0]), true);
    ASSERT_EQ(cache.accessMem(memaccess[8]), false);
    ASSERT_EQ(cache.accessMem(memaccess[0]), false);

    ASSERT_EQ(cache.accessMem(memaccess[2]), true);
    ASSERT_EQ(cache.accessMem(memaccess[9]), false);
    ASSERT_EQ(cache.accessMem(memaccess[2]), false);
}


TEST(FullyAssociativeCache, noEntry_Miss_LRU)
{
    LRURepPolicy policy(1, 8);
    FullyAssociativeCache cache(8, 32, &policy);
    auto memaccess = MemAccessDesc{MemOperation::Load, 0x1234567};
    ASSERT_EQ(cache.accessMem(memaccess), false);
}

TEST(FullyAssociativeCache, several_Addresses_LRU)
{
    LRURepPolicy policy(1, 8);
    FullyAssociativeCache cache(8, 32, &policy);
    std::vector<MemAccessDesc> memaccess = {
        MemAccessDesc{MemOperation::Load, 0x1000000},
        MemAccessDesc{MemOperation::Load, 0x2000000},
        MemAccessDesc{MemOperation::Load, 0x3000000},
        MemAccessDesc{MemOperation::Load, 0x4000000},
        MemAccessDesc{MemOperation::Load, 0x5000000},
        MemAccessDesc{MemOperation::Load, 0x6000000},
        MemAccessDesc{MemOperation::Load, 0x7000000},
        MemAccessDesc{MemOperation::Load, 0x8000000},
        MemAccessDesc{MemOperation::Load, 0x9000000},
        MemAccessDesc{MemOperation::Load, 0xA000000}
    };

    for(auto i = 0; i < 8; i++)
        ASSERT_EQ(cache.accessMem(memaccess[i]), false);

    ASSERT_EQ(cache.accessMem(memaccess[0]), true);
    ASSERT_EQ(cache.accessMem(memaccess[8]), false);
    ASSERT_EQ(cache.accessMem(memaccess[1]), false);

    ASSERT_EQ(cache.accessMem(memaccess[3]), true);
    ASSERT_EQ(cache.accessMem(memaccess[9]), false);
    ASSERT_EQ(cache.accessMem(memaccess[4]), false);
}