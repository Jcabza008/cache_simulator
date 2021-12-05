#pragma once

namespace cache_simulator
{

    class ICache
    {
        public:
        virtual ~ICache() {}
        virtual bool accessMem(MemAccessDesc& desc) = 0;
    };

    class DirectMapCache : public ICache
    {
        public:
        DirectMapCache(uint32_t lineCount, uint32_t lineSize)
            : lineSize(lineSize), storage(std::vector<int64_t>(lineCount, -1)) {}

        bool accessMem(MemAccessDesc& desc) override;

        private:
        DirectMapCache();
        int64_t getTag(uint32_t addr);

        const uint32_t lineSize;
        std::vector<int64_t> storage;
    };

    class FullyAssociativeCache : public ICache
    {
        public:
        FullyAssociativeCache(uint32_t lineCount, uint32_t lineSize, IReplacementPolicy* repPolicy)
            : lineSize(lineSize), storage(std::vector<int64_t>(lineCount, -1)), repPolicy(repPolicy) {}

        bool accessMem(MemAccessDesc& desc) override;

        private:
        FullyAssociativeCache();
        int64_t getTag(uint32_t addr);

        const uint32_t lineSize;
        std::vector<int64_t> storage;
        IReplacementPolicy* const repPolicy;
    };

    class SetAssociativeCache : public ICache
    {
        public:
        SetAssociativeCache(uint32_t setCount, uint32_t linesPerSet, uint32_t lineSize, IReplacementPolicy* repPolicy)
            : lineSize(lineSize), storage(std::vector<std::vector<int64_t>>(setCount, std::vector<int64_t>(linesPerSet, -1))), repPolicy(repPolicy) {}

        bool accessMem(MemAccessDesc& desc) override;

        private:
        SetAssociativeCache();
        int64_t getTag(uint32_t addr);

        const uint32_t lineSize;
        std::vector<std::vector<int64_t>> storage;
        IReplacementPolicy* const repPolicy;
    };
}