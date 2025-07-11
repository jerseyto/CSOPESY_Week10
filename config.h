#ifndef CONFIG_H
#define CONFIG_H
#include <string>


struct Config {
    int numCpu = 2;
    std::string scheduler = "rr";
    int quantumCycles = 4;
    int batchProcessFreq = 1;
    int minInstructions = 100;
    int maxInstructions = 100;
    int maxOverallMem = 16384;
    int memPerFrame = 16;
    int memPerProc = 4096;
};

extern Config config;

#endif
#pragma once
