#include "format.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string minutes;
  string hours;
  string left_over_seconds;
  std::ostringstream formatted_time;
  long total_minutes = (float)seconds / 60;
  hours = std::to_string(total_minutes / 60);
  minutes = std::to_string(total_minutes % 60);
  left_over_seconds = std::to_string(seconds % 60);
  formatted_time << std::setfill('0') << std::setw(2) << hours << ":"
                 << std::setfill('0') << std::setw(2) << minutes << ":"
                 << std::setfill('0') << std::setw(2) << left_over_seconds;
  return formatted_time.str();
}
