#include <gtest/gtest.h>

#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>

#include "scanner.h"

using namespace cache_simulator;

TEST(Scanner, emptyStream)
{
    Scanner scanner;
    std::stringstream ss;
    auto res = scanner.scan(ss);
    ASSERT_EQ(res.size(), 0);
}

TEST(Scanner, oneEntry)
{
    Scanner scanner;
    std::stringstream ss("l 0x00000001 1\n");
    auto res = scanner.scan(ss);
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0].operation, MemOperation::Load);
    ASSERT_EQ(res[0].address, 1);
}

TEST(Scanner, threeEntries)
{
    Scanner scanner;
    std::stringstream ss("l 0x00000001 1\ns 0x000000FF 1\nl 0x0000000A 1\n");
    auto res = scanner.scan(ss);
    ASSERT_EQ(res.size(), 3);
    ASSERT_EQ(res[0].operation, MemOperation::Load);
    ASSERT_EQ(res[0].address, 1);
    ASSERT_EQ(res[1].operation, MemOperation::Store);
    ASSERT_EQ(res[1].address, 255);
    ASSERT_EQ(res[2].operation, MemOperation::Load);
    ASSERT_EQ(res[2].address, 10);
}