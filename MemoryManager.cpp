#include "MemoryManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>

MemoryManager::MemoryManager(int maxMem, int memPerFrame, int memPerProc)
    : memPerFrame(memPerFrame), memPerProc(memPerProc)
{
    totalFrames = maxMem / memPerFrame;
    frameTable.resize(totalFrames, {-1});
}

// First-Fit Allocation  
int MemoryManager::allocateMemory(int processId) {
    int framesNeeded = memPerProc / memPerFrame;
    int freeCount = 0;
    int startIndex = -1;

    for (int i = 0; i < totalFrames; ++i) {
        if (frameTable[i].processId == -1) {
            if (freeCount == 0)
                startIndex = i;
            freeCount++;
            if (freeCount == framesNeeded) {
                // allocate frames to process
                for (int j = startIndex; j < startIndex + framesNeeded; ++j)
                    frameTable[j].processId = processId;
                return startIndex * memPerFrame; // Return starting address
            }
        } else {
            freeCount = 0;
        }
    }

    // No sufficient contiguous memory block found
    return -1;
}

// Deallocate memory of a process  
void MemoryManager::deallocateMemory(int processId) {
    for (auto& frame : frameTable)
        if (frame.processId == processId)
            frame.processId = -1;
}

// External Fragmentation: sum of all free regions < memPerProc
int MemoryManager::calculateExternalFragmentation() {
    int fragmentation = 0;
    int freeCount = 0;

    for (int i = 0; i < totalFrames; ++i) {
        if (frameTable[i].processId == -1)
            freeCount++;
        else {
            if (freeCount * memPerFrame < memPerProc)
                fragmentation += freeCount * memPerFrame;
            freeCount = 0;
        }
    }

    // Check if last region is a fragment
    if (freeCount * memPerFrame < memPerProc)
        fragmentation += freeCount * memPerFrame;

    return fragmentation;
}

// Number of processes currently in memory
int MemoryManager::getProcessesInMemoryCount() {
    std::vector<int> uniqueProcessIds;
    for (auto& frame : frameTable) {
        if (frame.processId != -1) {
            if (std::find(uniqueProcessIds.begin(), uniqueProcessIds.end(), frame.processId) == uniqueProcessIds.end())
                uniqueProcessIds.push_back(frame.processId);
        }
    }
    return uniqueProcessIds.size();
}

// Print memory layout
void MemoryManager::printMemorySnapshot(int quantumCycle) {
    std::ofstream outFile("memory_stamp_" + std::to_string(quantumCycle) + ".txt");

    // Timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t timeStamp = std::chrono::system_clock::to_time_t(now);
    outFile << "Timestamp: " << std::put_time(std::localtime(&timeStamp), "%F %T") << "\n";

    outFile << "Number of processes in memory: " << getProcessesInMemoryCount() << "\n";
    outFile << "Total external fragmentation: " << calculateExternalFragmentation() << " bytes\n\n";

    outFile << "Memory:\n";

    int startAddr = 0;
    int currentPid = frameTable[0].processId;

    for (int i = 1; i <= totalFrames; ++i) {
        if (i == totalFrames || frameTable[i].processId != currentPid) {
            int endAddr = (i * memPerFrame) - 1;
            if (currentPid == -1)
                outFile << "[" << startAddr << " - " << endAddr << "] : Free\n";
            else
                outFile << "[" << startAddr << " - " << endAddr << "] : Process " << currentPid << "\n";

            if (i < totalFrames) {
                startAddr = i * memPerFrame;
                currentPid = frameTable[i].processId;
            }
        }
    }

    outFile.close();
}
