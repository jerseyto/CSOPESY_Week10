#include "MemoryManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ctime>

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

void MemoryManager::printMemorySnapshot(int quantumCycle) {
    std::ofstream outFile("memory_stamp-" + std::to_string(quantumCycle) + ".txt");

    // Timestamp formatting
    auto now = std::chrono::system_clock::now();
    std::time_t timeStamp = std::chrono::system_clock::to_time_t(now);
    std::tm timeInfo;
    localtime_s(&timeInfo, &timeStamp);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "(%m/%d/%Y %I:%M:%S%p)", &timeInfo);
    outFile << "Timestamp: " << buffer << "\n";

    outFile << "Number of processes in memory: " << getProcessesInMemoryCount() << "\n";
    outFile << "Total external fragmentation in KB: " << calculateExternalFragmentation() / 1024 << "\n";
    outFile << "----end--- = " << totalFrames * memPerFrame << "\n\n";

    // Print from top (end) to bottom (start)
    int i = totalFrames - 1;
    while (i >= 0) {
        int currentPid = frameTable[i].processId;
        int endAddr = (i + 1) * memPerFrame;
        int count = 1;

        // Count how many frames belong to this process/free block
        while (i - 1 >= 0 && frameTable[i - 1].processId == currentPid) {
            --i;
            ++count;
        }

        int startAddr = endAddr - (count * memPerFrame);

        outFile << endAddr << "\n";
        if (currentPid == -1)
            outFile << "Free\n";
        else
            outFile << "P" << currentPid << "\n";
        outFile << startAddr << "\n\n";

        --i;
    }

    outFile << "----start--- = 0\n";
    outFile.close();
}
