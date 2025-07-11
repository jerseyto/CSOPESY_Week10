#include "Process.h"
#include <sstream>
#include <random>

Process::Process(const std::string& name, int pid, int instructionCount)
    : name(name), pid(pid), state(READY), instructionPointer(0) {
    generateInstructions(instructionCount);
    startTime = std::chrono::system_clock::now();
}

void Process::generateInstructions(int count) {
    static const std::vector<std::string> ops = {
        "ADD", "SUB", "MUL", "DIV", "PRINT", "DECLARE"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> opDist(0, ops.size() - 1);
    std::uniform_int_distribution<> varDist(0, 10);
    std::uniform_int_distribution<> valDist(1, 100);

    for (int i = 0; i < count; ++i) {
        std::ostringstream instr;
        std::string op = ops[opDist(gen)];

        if (op == "DECLARE") {
            instr << "DECLARE: var" << varDist(gen) << " = " << valDist(gen);
        }
        else if (op == "PRINT") {
            instr << "PRINT: var" << varDist(gen);
        }
        else {
            instr << op << ": var" << varDist(gen) << ", var" << varDist(gen);
        }

        instructions.push_back(instr.str());
    }
}

bool Process::isFinished() const {
    return instructionPointer >= static_cast<int>(instructions.size());
}
