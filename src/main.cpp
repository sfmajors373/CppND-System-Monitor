#include <iostream>

#include "ncurses_display.h"
#include "process.h"
#include "processor.h"
#include "system.h"

int main() {
  System system;
  Processor processor;
  Process process = Process(1);
  // NCursesDisplay::Display(system);
  std::cout << "System Uptime: " << system.UpTime() << std::endl;
  std::cout << "System kernel: " << system.Kernel() << std::endl;
  std::cout << "System memory utilization: " << system.MemoryUtilization()
            << std::endl;
  std::cout << "System os: " << system.OperatingSystem() << std::endl;
  std::cout << "System running processes: " << system.RunningProcesses()
            << std::endl;
  std::cout << "System total processes: " << system.TotalProcesses()
            << std::endl;
  std::cout << "Processor utilization: " << processor.Utilization()
            << std::endl;
  std::cout << "Process Command: " << process.Command() << std::endl;
  std::cout << "Process Ram: " << process.Ram() << std::endl;
}
