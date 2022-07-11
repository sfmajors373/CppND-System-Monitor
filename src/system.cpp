#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
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

// Return the system's CPU
Processor& System::Cpu() {
  Processor cpu_;
  return cpu_;
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for (int i = 0; i < pids.size(); i++) {
    Process process(pids[i]);
    processes_.push_back(process);
  }
  return processes_;
}

// Return the system's kernel identifier (string)
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

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() {
  string line;
  string os_name;
  std::vector<int> quote_positions;
  std::ifstream os_file("/etc/os-release");
  while (getline(os_file, line)) {
    // std::cout << "line: " << line << std::endl;
    // std::cout << "line.find(): " << line.find("PRETTY_NAME") << std::endl;
    if (line.find("PRETTY_NAME") != std::string::npos) {
      // std::cout << "pretty name: " << line << std::endl;
      for (int i = 0; i < line.length(); i++) {
        if (line[i] == '"') {
          quote_positions.push_back(i);
        }
      }
      int length = line.length() - quote_positions[0];
      os_name = line.substr(quote_positions[0] + 1, length - 2);
    }
  }
  return os_name;
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
