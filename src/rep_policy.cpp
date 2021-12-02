#include <vector>
#include <queue>
#include <sstream>
#include <cmath>
#include <stdint.h>
#include <chrono>
#include <sys/time.h>
#include <ctime>


#include "scanner.h"
#include "rep_policy.h"

using namespace cache_simulator;

void FIFORepPolicy::stored(uint32_t set, int64_t line)
{
    this->queues[set].push(line);
}

int64_t FIFORepPolicy::replace(uint32_t set)
{
    auto next = this->queues[set].front();
    this->queues[set].pop();
    return next;
}

void LRURepPolicy::stored(uint32_t set, int64_t line)
{
    auto millis = std::chrono::system_clock::now().time_since_epoch().count();
    this->times[set][line] = millis;
}

int64_t LRURepPolicy::replace(uint32_t set)
{
    uint32_t minIndex = -1;
    int64_t min = INT64_MAX;
    for(size_t i = 0; i < this->times[set].size(); i++)
    {
        if(this->times[set][i] > 0 && this->times[set][i] < min)
        {
            min = this->times[set][i];
            minIndex = i;
        }
    }
    this->times[set][minIndex] = -1;

    return minIndex;
}