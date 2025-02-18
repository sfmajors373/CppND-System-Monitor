#include "process.h"

#include <unistd.h>

#include <cctype>
#include <fstream>
#include <iomanip>
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
float Process::CpuUtilization() const {
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
  float cpu_usage;
  long jiffies;
  // std::cout << "System Uptime: " << system_uptime << std::endl;
  if (stat_file.is_open()) {
    getline(stat_file, line);
    for (long unsigned int i = 0; i < line.length(); i++) {
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
    cpu_usage = (((float)total_time / sysconf(_SC_CLK_TCK)) / seconds);
  }
  return cpu_usage;
}

// Return the command that generated this process
string Process::Command() {
  string command = LinuxParser::Command(this->pid_);
  if (command.length() < 40) {
    return command;
  } else {
    // Limit the length of the command shown to 40 characters
    return command.substr(0, 40) + "...";
  }
}

// Return this process's memory utilization
string Process::Ram() {
  string ram = LinuxParser::Ram(this->pid_);
  int decimal_place;
  for (long unsigned int i = 0; i < ram.length(); i++) {
    if (ram[i] == '.') {
      decimal_place = i;
    }
  }
  return ram.substr(0, decimal_place + 3);
}

// Return the user (name) that generated this process
string Process::User() {
  string user = LinuxParser::User(this->pid_);
  if (user.length() < 7) {
    return user;
  } else {
    // Limit the length of the username
    // else it extends into CPU%
    return user.substr(0, 7);
  }
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime() - LinuxParser::UpTime(this->pid_);
}

// Overload the "less than" comparison operator for Process objects
// Sorting by CPU Utilization
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}
