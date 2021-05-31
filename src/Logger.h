#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

struct LogEntry {
  int type;
  std::string message;
};

class Logger {
public:
  static std::vector<LogEntry> messages;
  static void Log(const std::string &message);
  static void Err(const std::string &message);
};
#endif