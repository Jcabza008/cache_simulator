#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <sstream>
#include <string>
#include <chrono>
#include <sys/time.h>
#include <ctime>

#include <boost/program_options.hpp>

#include "config.h"
#include "rep_policy.h"
#include "scanner.h"
#include "cache.h"

using namespace cache_simulator;
namespace po = boost::program_options;

 // Benchmark configs
std::vector<uint32_t> directMapSetCounts = {1, 2, 4, 8, 16, 32, 64, 128, 256};
std::vector<uint32_t> directMapLineSizes = {4, 8, 16, 32, 64, 128, 256};

std::vector<int> fullyAssociativeRepPolicies = {0, 1};
std::vector<uint32_t> fullyAssociativeLinesPerSet = {1, 2, 4, 8, 16, 32, 64, 128, 256};
std::vector<uint32_t> fullyAssociativeLineSizes = {4, 8, 16, 32, 64, 128, 256};

std::vector<int> setAssociativeRepPolicies = {0, 1};
std::vector<uint32_t> setAssociativeSetCount = {1, 2, 4, 8, 16, 32, 64, 128, 256};
std::vector<uint32_t> setAssociativeLinesPerSet = {1, 2, 4, 8, 16, 32, 64, 128, 256};
std::vector<uint32_t> setAssociativeLineSizes = {4, 8, 16, 32, 64, 128, 256};

double run(CacheSimulatorConfig config)
{
    IReplacementPolicy* repPolicy = nullptr;
    switch(config.repPolicy)
    {
        case 0:
            repPolicy = new FIFORepPolicy(config.setCount);
            break;
        case 1:
            repPolicy = new LRURepPolicy(config.setCount, config.linesPerSet);
            break;
        default:
            std::cout << "Invalid replacement policy" << std::endl;
            return -1;
    }

    ICache* cache = nullptr;
    switch(config.cacheType)
    {
        case 0:
            cache = new FullyAssociativeCache(config.linesPerSet, config.lineSize, repPolicy);
            break;
        case 1:
            cache = new DirectMapCache(config.setCount, config.lineSize);
            break;
        case 2:
            cache = new SetAssociativeCache(config.setCount, config.linesPerSet, config.lineSize, repPolicy);
            break;
        default:
            std::cout << "Invalid cache type" << std::endl;
            return -1;
    }

    std::fstream file(config.traceFile, std::ios::in);

    Scanner scanner;
    auto trace = scanner.scan(file);

    double total = trace.size();
    double hits = 0;

    for(auto it = trace.begin(); it != trace.end(); it++)
    {
        hits += cache->accessMem(*it) ? 1 : 0;
    }

    file.close();
    delete cache;
    delete repPolicy;

    return hits/total;
}

void benchmark(std::string traceFile)
{
    std::string base_filename = traceFile.substr(traceFile.find_last_of("/\\") + 1);
    std::vector<CacheSimulatorRunStats> stats;

    // Direct Map
    for(size_t i = 0; i < directMapSetCounts.size(); i++)
    {
        for(size_t j = 0; j < directMapLineSizes.size(); j++)
        {
            auto config = CacheSimulatorConfig{
                traceFile,
                1,
                0,
                directMapSetCounts[i],
                1,
                directMapLineSizes[j]
            };

            double hitRate = run(config);
            stats.push_back(CacheSimulatorRunStats{
                base_filename,
                "Direct Map",
                "N/A",
                directMapSetCounts[i],
                1,
                directMapLineSizes[j],
                hitRate
            });
        }
    }

    // Fully Associative
    for(size_t i = 0; i < fullyAssociativeRepPolicies.size(); i++)
    {
        for(size_t j = 0; j < fullyAssociativeLinesPerSet.size(); j++)
        {
            for(size_t k = 0; k < fullyAssociativeLineSizes.size(); k++)
            {
                auto config = CacheSimulatorConfig{
                    traceFile,
                    0,
                    fullyAssociativeRepPolicies[i],
                    1,
                    fullyAssociativeLinesPerSet[j],
                    fullyAssociativeLineSizes[k]
                };

                double hitRate = run(config);
                stats.push_back(CacheSimulatorRunStats{
                    base_filename,
                    "Fully Associative",
                    fullyAssociativeRepPolicies[i] == 0 ? "FIFO" : "LRU",
                    1,
                    fullyAssociativeLinesPerSet[j],
                    fullyAssociativeLineSizes[k],
                    hitRate
                });
            }
        }
    }

    // Set Associative
    for(size_t i = 0; i < setAssociativeRepPolicies.size(); i++)
    {
        for(size_t j = 0; j < setAssociativeSetCount.size(); j++)
        {
            for(size_t k = 0; k < setAssociativeLinesPerSet.size(); k++)
            {
                for(size_t l = 0; l < setAssociativeLineSizes.size(); l++)
                {
                    auto config = CacheSimulatorConfig{
                        traceFile,
                        2,
                        setAssociativeRepPolicies[i],
                        setAssociativeSetCount[k],
                        setAssociativeLinesPerSet[j],
                        setAssociativeLineSizes[l]
                    };

                    double hitRate = run(config);
                    stats.push_back(CacheSimulatorRunStats{
                        base_filename,
                        "Set Associative",
                        setAssociativeRepPolicies[i] == 0 ? "FIFO" : "LRU",
                        setAssociativeSetCount[k],
                        setAssociativeLinesPerSet[j],
                        setAssociativeLineSizes[l],
                        hitRate
                    });
                }
            }
        }
    }

    std::ofstream out("benchmark_stats_" + base_filename + ".csv");
    out << "File,Cache Type,Replacement Policy,Set Count,Lines/Set,Line Size,Hit Rate" << std::endl;

    for(size_t i = 0; i < stats.size(); i++)
    {
        out << stats[i].file << ",";
        out << stats[i].cacheType << ",";
        out << stats[i].repPolicy << ",";
        out << stats[i].setCount << ",";
        out << stats[i].linesPerSet << ",";
        out << stats[i].lineSize << ",";
        out << stats[i].hitRate << std::endl;
    }

    out.close();
}

int main(int argc, char** argv)
{
    CacheSimulatorConfig config = {};

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("trace-file", po::value<std::string>(&config.traceFile), "trace file to run (required)")
        ("cache-type", po::value<int>(&config.cacheType)->default_value(1), "cache type. Supported: fully associative = 0, direct map = 1, set associative = 2")
        ("rep-policy", po::value<int>(&config.repPolicy)->default_value(0), "replacement policy. Supported: FIFO = 0, LRU = 1")
        ("set-count", po::value<uint32_t>(&config.setCount)->default_value(32), "number of lines in the cache")
        ("lines-per-set", po::value<uint32_t>(&config.linesPerSet)->default_value(1), "number of lines in each set")
        ("line-size", po::value<uint32_t>(&config.lineSize)->default_value(8), "number of bytes per line")
        ("benchmark", "run default benchmark")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        desc.print(std::cout);
        return 0;
    }

    if(!vm.count("trace-file"))
    {
        std::cout << "Parameter --trace-file required" << std::endl;
        return -1;
    }

    if(vm.count("benchmark"))
    {
        benchmark(config.traceFile);
        return 0;
    }

    std::cout << run(config) << std::endl;

    return 0;
}