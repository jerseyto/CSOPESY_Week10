#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

enum ProcessState {
    READY,
    RUNNING,
    FINISHED
};

class Process {
public:
    std::string name;
    int pid;
    ProcessState state;
    std::vector<std::string> instructions;
    int instructionPointer;
    int memoryStart = -1;
    int memoryEnd = -1;
    std::unordered_map<std::string, uint16_t> memory;
    std::chrono::system_clock::time_point startTime;

    Process(const std::string& name, int pid, int instructionCount);

    void generateInstructions(int count);
    bool isFinished() const;
};

#endif
#pragma once
