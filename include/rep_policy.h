#pragma once

namespace cache_simulator
{

    class IReplacementPolicy
    {
        public:
        virtual ~IReplacementPolicy() {}

        virtual void used(uint32_t set, int64_t line) = 0;
        virtual void stored(uint32_t set, int64_t tag) = 0;
        virtual int64_t replace(uint32_t set) = 0;
    };

    class FIFORepPolicy : public IReplacementPolicy
    {
        public:
        FIFORepPolicy(uint32_t setNumber)
            : queues(std::vector<std::queue<int64_t>>(setNumber, std::queue<int64_t>())) {}

        void used(uint32_t set, int64_t line) override {}
        void stored(uint32_t set, int64_t line) override;
        int64_t replace(uint32_t set) override;

        private:
        FIFORepPolicy();

        std::vector<std::queue<int64_t>> queues;
    };

    class LRURepPolicy : public IReplacementPolicy
    {
        public:
        LRURepPolicy(uint32_t setNumber, uint32_t setSize)
            : times(std::vector<std::vector<time_t>>(setNumber, std::vector<int64_t>(setSize, -1))) {}

        void used(uint32_t set, int64_t line) override { this->stored(set, line); }
        void stored(uint32_t set, int64_t line) override;
        int64_t replace(uint32_t set) override;

        private:
        LRURepPolicy();

        std::vector<std::vector<time_t>> times;
    };

}