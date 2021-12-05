#pragma once

namespace cache_simulator
{
    class CacheSimulatorConfig
    {
        public:

        // CacheType
        // FullyAssociative = 0
        // DirectMap = 1
        // SetAssociative = 2

        // RepPolicy
        // FIFO = 0
        // LRU = 1

        std::string traceFile;
        int cacheType;
        int repPolicy;
        uint32_t setCount;
        uint32_t linesPerSet;
        uint32_t lineSize;
    };

    struct CacheSimulatorRunStats
    {
        std::string file;
        std::string cacheType;
        std::string repPolicy;
        uint32_t setCount;
        uint32_t linesPerSet;
        uint32_t lineSize;
        double hitRate;
    };
}