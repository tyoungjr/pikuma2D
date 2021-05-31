#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

// why C is important
std::string CurrentDateTimeToString() {
  std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock ::now());
  std::string output(30, '\0');
  std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S",
                std::localtime(&now));

  return output;
}
void Logger::Log(const std::string &message) {
  std::string output = "LOG: [" + CurrentDateTimeToString() + "]" + message;
  std::cout << "\x1B[32m" << output << "\033[0m" << std::endl;
}

void Logger::Err(const std::string &message) {
  std::string output = "Err: [" + CurrentDateTimeToString() + "]" + message;
  std::cerr << "\x1B[91m" << output << "\033[0m" << std::endl;
}