
#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>



std::vector<LogEntry> Logger::messages;
// why C is important
std::string CurrentDateTimeToString() {
  std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock ::now());
  std::string output(30, '\0');

  // localtime is unsafe but idk what to change it to
  // see https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4996?view=msvc-170`
  std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S",
                std::localtime(&now));

  return output;
}
void Logger::Log(const std::string &message) {
  LogEntry logEntry;
  logEntry.type = LOG_INFO;
  logEntry.message = "LOG: [" + CurrentDateTimeToString() + "]" + message;

  std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
  messages.push_back(logEntry);
}

void Logger::Err(const std::string &message) {
  LogEntry logEntry;
  logEntry.type = LOG_ERROR;
  logEntry.message = "ERR: [" + CurrentDateTimeToString() + "]" + message;

  std::cerr << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;
  messages.push_back(logEntry);
}