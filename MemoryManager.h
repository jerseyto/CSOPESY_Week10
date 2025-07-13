#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>
#include <string>

struct Frame {
    int processId; // -1 means free
};

class MemoryManager {
private:
    int totalFrames;
    int memPerFrame;
    int memPerProc;
    std::vector<Frame> frameTable;

public:
    MemoryManager(int maxMem, int memPerFrame, int memPerProc);

    int allocateMemory(int processId);
    void deallocateMemory(int processId);

    int calculateExternalFragmentation();
    void printMemorySnapshot(int quantumCycle);

    int getProcessesInMemoryCount();
};

#endif
