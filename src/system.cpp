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

// Return the system's memory utilization
float System::MemoryUtilization() {
  // total used mem = memTotal - MemFree
  // non-cache/buffer mem (green) = total used mem - (buffers + cached)
  // buffers (blue) = Buffers
  // Cached memory (yellow) = Cached + SReclaimable - Shmem
  // Swap = SwapTotal - SwapFree
  string mem_total_line;
  string mem_free_line;
  std::vector<int> space_positions;
  std::vector<int> space_positions2;
  std::ifstream meminfo_file("/proc/meminfo");
  if (meminfo_file.is_open()) {
    getline(meminfo_file, mem_total_line);
    getline(meminfo_file, mem_free_line);
  }
  // mem total
  for (int i = 0; i < mem_total_line.length(); i++) {
    if (mem_total_line[i] == ' ') {
      space_positions.push_back(i);
    }
  }
  int len = space_positions.size();
  int word_end = space_positions[len - 1];
  int word_begin = space_positions[len - 2];
  string mem_total_str =
      mem_total_line.substr(word_begin, word_end - word_begin);
  int mem_total_int = std::stoi(mem_total_str);
  // std::cout << "Mem total: " << mem_total_str << std::endl;

  // mem free
  for (int i = 0; i < mem_free_line.length(); i++) {
    if (mem_free_line[i] == ' ') {
      space_positions2.push_back(i);
    }
  }
  len = space_positions2.size();
  word_end = space_positions2[len - 1];
  word_begin = space_positions2[len - 2];
  string mem_free_str = mem_free_line.substr(word_begin, word_end - word_begin);
  int mem_free_int = std::stoi(mem_free_str);
  // std::cout << "Mem free: " << mem_free_str << std::endl;
  float percent_mem_utilization =
      ((mem_total_int - mem_free_int) / mem_total_int) * 100.0;
  return mem_total_int * 1.0;
}

// Return the operating system name
std::string System::OperatingSystem() {
  string line;
  string os_name;
  std::vector<int> quote_positions;
  std::ifstream os_file("/etc/os-release");
  if (os_file.is_open()) {
    getline(os_file, line);
    if (line.find("PRETTY_NAME")) {
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
