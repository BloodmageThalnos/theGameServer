#include <iostream>
#include <sstream>
#include <string>

#define LOG(msg)                                           \
  do {                                                     \
    std::cout << __FUNCTION__ << ": " << msg << std::endl; \
  } while (0)

#define ERROR(msg)                                      \
  do {                                                  \
    std::cout << __PRETTY_FUNCTION__ << "found error: " \
              << "\n\t" << msg << std::endl;            \
  } while (0)