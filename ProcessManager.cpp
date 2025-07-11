#include "ProcessManager.h"
#include "config.h"
#include <sstream>

std::shared_ptr<Process> ProcessManager::generateRandomProcess() {
    std::ostringstream nameStream;
    nameStream << "p" << pNameCounter++;
    std::string name = nameStream.str();

    return generateRandomProcess(name);
}

std::shared_ptr<Process> ProcessManager::generateRandomProcess(const std::string& name) {
    auto proc = std::make_shared<Process>(name, pidCounter++, config.minInstructions); // config from config.h
    allProcesses.push_back(proc);
    return proc;
}
