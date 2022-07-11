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
string Process::Command() { return LinuxParser::Command(this->pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

// Overload the "less than" comparison operator for Process objects
// Sorting by CPU Utilization
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}
