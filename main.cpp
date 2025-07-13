#include "ProcessManager.h"
#include "MemoryManager.h"
#include "config.h"
#include <iostream>

int main() {
    ProcessManager pm;

    // Generate 3 test processes
    for (int i = 0; i < 3; ++i) {
        auto proc = pm.generateRandomProcess();
        std::cout << "Created process: " << proc->name << ", PID: " << proc->pid << std::endl;
        std::cout << "Instructions:\n";
        for (const auto& instr : proc->instructions) {
            std::cout << "  " << instr << "\n";
        }
        std::cout << std::endl;
    }

    // test for mem manager
    MemoryManager memManager(16384, 16, 4096);

    // Allocate 4 processes (should fit perfectly)
    for (int i = 1; i <= 4; i++) {
        int addr = memManager.allocateMemory(i);
        if (addr == -1) {
            std::cout << "Failed to allocate memory for Process " << i << "\n";
        } else {
            std::cout << "Allocated Process " << i << " at address " << addr << "\n";
        }
    }

    // Check external fragmentation (should be 0)
    int frag = memManager.calculateExternalFragmentation();
    std::cout << "External Fragmentation: " << frag << " bytes\n";

    // Print snapshot
    memManager.printMemorySnapshot(1);

    // Deallocate Process 2 and check fragmentation again
    memManager.deallocateMemory(2);
    frag = memManager.calculateExternalFragmentation();
    std::cout << "External Fragmentation after deallocation: " << frag << " bytes\n";

    // Print another snapshot
    memManager.printMemorySnapshot(2);

    // Attempt to allocate a new process (should fit in Process 2's space)
    int addrNew = memManager.allocateMemory(5);
    if (addrNew == -1) {
        std::cout << "Failed to allocate memory for Process 5\n";
    } else {
        std::cout << "Allocated Process 5 at address " << addrNew << "\n";
    }

    // Final snapshot
    memManager.printMemorySnapshot(3);

    return 0;
}
