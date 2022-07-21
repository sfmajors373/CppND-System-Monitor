#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
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
  for (long unsigned int i = 0; i < mem_total_line.length(); i++) {
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

  // mem free
  for (long unsigned int i = 0; i < mem_free_line.length(); i++) {
    if (mem_free_line[i] == ' ') {
      space_positions2.push_back(i);
    }
  }
  len = space_positions2.size();
  word_end = space_positions2[len - 1];
  word_begin = space_positions2[len - 2];
  string mem_free_str = mem_free_line.substr(word_begin, word_end - word_begin);
  int mem_free_int = std::stoi(mem_free_str);
  return ((float)(mem_total_int - mem_free_int) / mem_total_int);
}

// Read and return the system uptime
long LinuxParser::UpTime() {
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

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  std::ifstream stat_file("/proc/" + std::to_string(pid) + "/stat");
  std::vector<int> space_positions;
  long int jiffies;
  string jiffies_string;
  while (getline(stat_file, line)) {
    for (long unsigned int i = 0; i < line.length(); i++) {
      if (line[i] == ' ') {
        space_positions.push_back(i);
      }
    }
    int word_length = space_positions[21] - space_positions[20];
    jiffies_string = line.substr(space_positions[20] + 1, word_length);
    jiffies = std::stol(jiffies_string);
  }
  return jiffies;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string process_number_string;
  std::vector<int> space_positions;
  std::ifstream stats_file("/proc/stat");
  while (getline(stats_file, line)) {
    if (line.find("processes") != std::string::npos) {
      for (long unsigned int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
          space_positions.push_back(i);
        }
      }
      int str_length = line.length() - space_positions[0];
      process_number_string = line.substr(space_positions[0], str_length);
    }
  }
  return std::stoi(process_number_string);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string process_number_string;
  std::vector<int> space_positions;
  std::ifstream stats_file("/proc/stat");
  while (getline(stats_file, line)) {
    if (line.find("procs_running") != std::string::npos) {
      for (long unsigned int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
          space_positions.push_back(i);
        }
      }
      int str_length = line.length() - space_positions[0];
      process_number_string = line.substr(space_positions[0], str_length);
    }
  }
  return std::stoi(process_number_string);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream command_file("/proc/" + std::to_string(pid) + "/cmdline");
  if (command_file.is_open()) {
    getline(command_file, line);
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  std::vector<int> space_positions;
  std::ifstream command_file("/proc/" + std::to_string(pid) + "/status");
  float mb_float = 0.0;
  while (getline(command_file, line)) {
    // Using VmRSS to reflect my phyical RAM size, not VM size
    if (line.find("VmRSS") != std::string::npos) {
      for (long unsigned int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
          space_positions.push_back(i);
        }
      }
      int spaces = space_positions.size();
      int length = space_positions[spaces - 1] - space_positions[spaces - 2];
      string kb_string = line.substr(space_positions[spaces - 2], length);
      int kb_int = std::stoi(kb_string);
      // convert kb to mb
      mb_float = (float)kb_int / 1000;
      return std::to_string(mb_float);
    }
  }
  return "0.0";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  std::ifstream status_file("/proc/" + std::to_string(pid) + "/status");
  std::vector<int> space_positions;
  string user;
  while (getline(status_file, line)) {
    if (line.find("Uid") != std::string::npos) {
      for (long unsigned int i = 0; i < line.length(); i++) {
        if (line[i] == '\t') {
          space_positions.push_back(i);
        }
      }
      int word_length = space_positions[1] - space_positions[0];
      user = line.substr(space_positions[0] + 1, word_length - 1);
    }
  }
  return user;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  std::ifstream etc_file("/etc/passwd");
  string uid = Uid(pid);
  string user;
  while (getline(etc_file, line)) {
    if (line.find(uid) != std::string::npos) {
      int colon_location = line.find(":");
      user = line.substr(0, colon_location);
    }
  }

  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long int jiffies = ActiveJiffies(pid);
  long seconds = jiffies / sysconf(_SC_CLK_TCK);
  return seconds;
}
