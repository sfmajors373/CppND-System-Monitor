#include "processor.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  string line;
  std::vector<int> space_positions;
  std::ifstream stat_file("/proc/stat");
  if (stat_file.is_open()) {
    getline(stat_file, line);
    int line_length = line.length();
    for (int i = 0; i < line_length; i++) {
      if (line[i] == ' ') {
        space_positions.push_back(i);
      }
    }
  }
  int word_size = space_positions[2] - space_positions[1];
  string user_string = line.substr(space_positions[1], word_size);
  // std::cout << "User: " << user_string << std::endl;
  int user = std::stoi(user_string);

  word_size = space_positions[3] - space_positions[2];
  string nice_string = line.substr(space_positions[2], word_size);
  // std::cout << "Nice: " << nice << std::endl;
  int nice = std::stoi(nice_string);

  word_size = space_positions[4] - space_positions[3];
  string system_string = line.substr(space_positions[3], word_size);
  // std::cout << "System: " << system_string << std::endl;
  int system = std::stoi(system_string);

  word_size = space_positions[5] - space_positions[4];
  string idle_string = line.substr(space_positions[4], word_size);
  // std::cout << "Idle: " << idle_string << std::endl;
  int idle = std::stoi(idle_string);

  word_size = space_positions[6] - space_positions[5];
  string iowait_string = line.substr(space_positions[5], word_size);
  // std::cout << "Iowait: " << iowait_string << std::endl;
  int iowait = std::stoi(iowait_string);

  word_size = space_positions[7] - space_positions[6];
  string irq_string = line.substr(space_positions[6], word_size);
  // std::cout << "Irq: " << irq_string << std::endl;
  int irq = std::stoi(irq_string);

  word_size = space_positions[8] - space_positions[7];
  string softirq_string = line.substr(space_positions[7], word_size);
  // std::cout << "softIrq: " << softirq_string << std::endl;
  int softirq = std::stoi(softirq_string);

  word_size = space_positions[9] - space_positions[8];
  string steal_string = line.substr(space_positions[8], word_size);
  // std::cout << "Steal: " << steal_string << std::endl;
  int steal = std::stoi(steal_string);

  word_size = space_positions[10] - space_positions[9];
  string guest_string = line.substr(space_positions[9], word_size);
  // std::cout << "Guest: " << guest_string << std::endl;
  int guest = std::stoi(guest_string);

  word_size = space_positions[11] - space_positions[10];
  string guest_nice_string = line.substr(space_positions[10], word_size);
  // std::cout << "Guest Nice: " << guest_nice_string << std::endl;
  int guest_nice = std::stoi(guest_nice_string);

  int Idle = idle + iowait;
  int NonIdle = user + nice + system + irq + softirq + steal;
  int Total = Idle + NonIdle;
  return ((float)Total - Idle) / Total;
}
