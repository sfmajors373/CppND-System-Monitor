#include "process.h"

#include <unistd.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  string line;
  std::ifstream stat_file("/proc/" + std::to_string(this->pid_) + "/stat");
  std::vector<int> space_positions;
  long system_uptime = LinuxParser::UpTime();
  string utime_string;
  string stime_string;
  string starttime_string;
  string jiffies_string;
  long utime;
  long stime;
  long starttime;
  float cpu_usage;
  long jiffies;
  // std::cout << "System Uptime: " << system_uptime << std::endl;
  if (stat_file.is_open()) {
    getline(stat_file, line);
    for (int i = 0; i < line.length(); i++) {
      if (line[i] == ' ') {
        space_positions.push_back(i);
      }
    }

    int utime_length = space_positions[13] - space_positions[12];
    utime_string = line.substr(space_positions[12] + 1, utime_length);
    // std::cout << "Utime string: " << utime_string << std::endl;

    int stime_length = space_positions[14] - space_positions[13];
    stime_string = line.substr(space_positions[12] + 1, stime_length);
    // std::cout << "Stime string: " << stime_string << std::endl;

    int word_length = space_positions[21] - space_positions[20];
    jiffies_string = line.substr(space_positions[20] + 1, word_length);
    // std::cout << "Jiffies string: " << jiffies_string << std::endl;

    utime = std::stol(utime_string);
    stime = std::stol(stime_string);
    jiffies = std::stol(jiffies_string);

    long total_time = utime + stime;
    long seconds = system_uptime - ((float)jiffies / sysconf(_SC_CLK_TCK));
    cpu_usage = 100 * (((float)total_time / sysconf(_SC_CLK_TCK)) / seconds);
  }
  return cpu_usage;
}

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
      // std::cout << "KB string: " << kb_string << std::endl;
      int kb_int = std::stoi(kb_string);
      // convert kb to mb
      float mb_float = (float)kb_int / 1000;
      return std::to_string(mb_float);
    }
  }
}

// Return the user (name) that generated this process
string Process::User() {
  string line;
  std::ifstream status_file("/proc/" + std::to_string(this->pid_) + "/status");
  std::vector<int> space_positions;
  string user;
  while (getline(status_file, line)) {
    if (line.find("Uid") != std::string::npos) {
      for (int i = 0; i < line.length(); i++) {
        if (line[i] == '\t') {
          space_positions.push_back(i);
        }
      }
      int word_length = space_positions[1] - space_positions[0];
      user = line.substr(space_positions[0] + 1, word_length);
    }
  }
  return user;
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
  string line;
  std::ifstream stat_file("/proc/" + std::to_string(this->pid_) + "/stat");
  std::vector<int> space_positions;
  long int uptime;
  long int jiffies;
  string jiffies_string;
  float almost_seconds;
  long int seconds;
  while (getline(stat_file, line)) {
    for (int i = 0; i < line.length(); i++) {
      if (line[i] == ' ') {
        space_positions.push_back(i);
      }
    }
    int word_length = space_positions[21] - space_positions[20];
    jiffies_string = line.substr(space_positions[20] + 1, word_length);
    jiffies = std::stol(jiffies_string);
    // std::cout << "Jiffies: " << jiffies << std::endl;
    almost_seconds = jiffies / sysconf(_SC_CLK_TCK);
    // seconds = Format::ElapsedTime(almost_seconds);
    // std::cout << "seconds: " << seconds << std::endl;
  }
  return almost_seconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// Sorting by CPU Utilization
bool Process::operator<(Process const& a) const {
  // if (a.CpuUtilization() > this.CpuUtilization()) {
  //   return true;
  // } else {
  //   return false;
  // }
  return true;
}
