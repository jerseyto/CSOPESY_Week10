#include "ProcessManager.h"
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

    return 0;
}
