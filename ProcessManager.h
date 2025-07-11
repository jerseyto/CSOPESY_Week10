#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "Process.h"
#include <vector>
#include <memory>
#include <string>

class ProcessManager {
private:
    int pidCounter = 0;
    int pNameCounter = 1;

public:
    std::vector<std::shared_ptr<Process>> allProcesses;

    std::shared_ptr<Process> generateRandomProcess();
    std::shared_ptr<Process> generateRandomProcess(const std::string& name);
};

#endif
#pragma once
