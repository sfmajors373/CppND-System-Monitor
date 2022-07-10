#include "format.h"

#include <iostream>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds [[maybe_unused]]) {
  string minutes;
  string hours;
  string left_over_seconds;
  long total_minutes = (float)seconds / 60;
  hours = std::to_string(total_minutes / 60);
  minutes = std::to_string(total_minutes % 60);
  left_over_seconds = std::to_string(seconds % 60);
  std::cout << "Hours: " << hours << std::endl;
  std::cout << "Minutes: " << minutes << std::endl;
  std::cout << "Seconds: " << left_over_seconds << std::endl;
  string formatted_time = hours + ":" + minutes + ":" + left_over_seconds;
  std::cout << "Formatted time: " << formatted_time << std::endl;
  return formatted_time;
}
