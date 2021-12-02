#include <vector>
#include <queue>
#include <sstream>
#include <cmath>
#include <stdint.h>

#include "rep_policy.h"
#include "scanner.h"
#include "cache.h"

using namespace cache_simulator;

constexpr uint64_t c_main_mem_size = 4294967296; // 2^32 bytes

bool DirectMapCache::accessMem(MemAccessDesc& desc)
{
    auto blkNum = desc.address / this->lineSize;
    auto lineNum = blkNum % this->storage.size();

    if(this->storage[lineNum] == this->getTag(desc.address))
    {
        return true;
    }

    this->storage[lineNum] = this->getTag(desc.address);
    return false;
}

int64_t DirectMapCache::getTag(uint32_t addr)
{
    auto offsetBits = static_cast<uint32_t>(std::log2(this->lineSize));
    auto lineBits = static_cast<uint32_t>(std::log2(this->storage.size()));
    return addr >> (offsetBits + lineBits);
}

bool FullyAssociativeCache::accessMem(MemAccessDesc& desc)
{
    auto tag = this->getTag(desc.address);
    for(size_t i = 0; i < this->storage.size(); i++)
    {
        if(storage[i] == tag)
        {
            this->repPolicy->used(0, i);
            return true;
        }
    }

    for(size_t i = 0; i < this->storage.size(); i++)
    {
        if(storage[i] == -1)
        {
            storage[i] = tag;
            this->repPolicy->stored(0, i);
            return false;
        }
    }

    auto freedLine = this->repPolicy->replace(0);
    this->storage[freedLine] = tag;
    this->repPolicy->stored(0, freedLine);

    return false;
}

int64_t FullyAssociativeCache::getTag(uint32_t addr)
{
    auto offsetBits = static_cast<uint32_t>(std::log2(this->lineSize));
    return addr >> offsetBits;
}

bool SetAssociativeCache::accessMem(MemAccessDesc& desc)
{
    auto blkNum = desc.address / this->lineSize;
    auto setNum = blkNum % this->storage.size();
    auto tag = this->getTag(desc.address);

    for(size_t i = 0; i < this->storage[setNum].size(); i++)
    {
        if(storage[setNum][i] == tag)
        {
            this->repPolicy->used(setNum, i);
            return true;
        }
    }

    for(size_t i = 0; i < this->storage[setNum].size(); i++)
    {
        if(storage[setNum][i] == -1)
        {
            storage[setNum][i] = tag;
            this->repPolicy->stored(setNum, i);
            return false;
        }
    }

    auto freedLine = this->repPolicy->replace(setNum);
    this->storage[setNum][freedLine] = tag;
    this->repPolicy->stored(setNum, freedLine);

    return false;
}

int64_t SetAssociativeCache::getTag(uint32_t addr)
{
    auto offsetBits = static_cast<uint32_t>(std::log2(this->lineSize));
    auto setBits = static_cast<uint32_t>(std::log2(this->storage.size()));
    return addr >> (offsetBits + setBits);
}