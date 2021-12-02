#pragma once

namespace cache_simulator
{

    enum MemOperation
    {
        Load,
        Store
    };

    struct MemAccessDesc
    {
        MemOperation operation;
        uint32_t address;
    };

    class IScanner
    {
        public:
        virtual ~IScanner() {}
        virtual std::vector<MemAccessDesc> scan(std::istream& is) = 0;
    };

    class Scanner : public IScanner
    {
        public:
        std::vector<MemAccessDesc> scan(std::istream& is) override;

        private:
        MemAccessDesc scanLine(std::string line);
    };

}