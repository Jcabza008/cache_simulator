#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>

#include "scanner.h"

using namespace cache_simulator;

constexpr uint8_t c_line_size = 14;
constexpr uint8_t c_addr_start = 2;
constexpr uint8_t c_addr_end = 12;
constexpr uint8_t c_addr_base = 16;

std::vector<MemAccessDesc> Scanner::scan(std::istream& is)
{
    std::vector<MemAccessDesc> res;
    std::string buff;
    while(std::getline(is, buff, '\n'))
        res.push_back(this->scanLine(buff));

    return res;
}

MemAccessDesc Scanner::scanLine(std::string line)
{
    auto op = line[0] == 'l' ? MemOperation::Load : MemOperation::Store;
    uint32_t addr = std::stoul(line.substr(c_addr_start, c_addr_end), nullptr, c_addr_base);

    return MemAccessDesc{
        op,
        addr
    };
}