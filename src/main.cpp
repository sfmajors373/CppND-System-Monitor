#include <iostream>

#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  // NCursesDisplay::Display(system);
  std::cout << "System Uptime: " << system.UpTime() << std::endl;
  std::cout << "System kernel: " << system.Kernel() << std::endl;
}
