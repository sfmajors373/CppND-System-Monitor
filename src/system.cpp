#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  string line;
  std::vector<int> space_positions;
  std::ifstream version_file("/proc/version");
  if (version_file.is_open()) {
    getline(version_file, line);
    int line_length = line.length();
    for (int i = 0; i < line_length; i++) {
      if (line[i] == ' ') {
        space_positions.push_back(i);
      }
    }
  }
  // fine length of kernel word 3rd space pos minus 2nd space
  int length_of_kernel_string = space_positions[2] - space_positions[1];
  // return 2nd space, length
  return line.substr(space_positions[1], length_of_kernel_string);
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  // grab first four lines of /proc/meminfo
  // total used mem = memTotal - MemFree
  // non-cache/buffer mem (green) = total used mem - (buffers + cached)
  // buffers (blue) = Buffers
  // Cached memory (yellow) = Cached + SReclaimable - Shmem
  // Swap = SwapTotal - SwapFree
  // Return float --> percentage???
  return 0.0;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { return string(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// Return the number of seconds since the system started running
long int System::UpTime() {
  // /proc/uptime first number
  string line;
  string uptime_string;
  long int uptime;
  string::size_type sz;
  std::ifstream uptime_file("/proc/uptime");
  if (uptime_file.is_open()) {
    getline(uptime_file, line);
    int i = 0;
    while (line[i] != ' ') {
      i = i + 1;
    }
    uptime_string = line.substr(0, i);
    uptime_file.close();
  }
  uptime = std::stol(uptime_string, &sz);
  return uptime;
}
