#include "process.h"

#include <unistd.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

// Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }
// string line;
// std::ifstream command_file("/proc/" + std::to_string(this->pid_) +
// "/cmdline");

// Return the command that generated this process
string Process::Command() {
  string line;
  std::ifstream command_file("/proc/" + std::to_string(this->pid_) +
                             "/cmdline");
  if (command_file.is_open()) {
    getline(command_file, line);
  }
  return line;
}

// Return this process's memory utilization
string Process::Ram() {
  string line;
  std::vector<int> space_positions;
  std::ifstream command_file("/proc/" + std::to_string(this->pid_) + "/status");
  while (getline(command_file, line)) {
    if (line.find("VmSize") != std::string::npos) {
      for (int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
          space_positions.push_back(i);
        }
      }
      int spaces = space_positions.size();
      int length = space_positions[spaces - 1] - space_positions[spaces - 2];
      string kb_string = line.substr(space_positions[spaces - 2], length);
      std::cout << "KB string: " << kb_string << std::endl;
      int kb_int = std::stoi(kb_string);
      // convert kb to mb
      float mb_float = (float)kb_int / 1000;
      return std::to_string(mb_float);
    }
  }
}

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }
// string line;
// std::ifstream command_file("/proc/" + std::to_string(this->pid_) +
// "/cmdline");

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }
// string line;
// std::ifstream command_file("/proc/" + std::to_string(this->pid_) +
// "/cmdline");

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}
