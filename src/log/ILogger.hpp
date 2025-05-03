
#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

namespace logger {

enum LogType { TRACE, INFO, WARNING, ERROR };

class ILogger {
protected:
  LogType filterLevel;

public:
  ILogger(){};
  virtual ~ILogger(){};
  virtual void print(LogType type, const std::string& message) = 0;
  virtual void run() = 0;
  virtual void shutdown() = 0;
  void setFilter(LogType level) { filterLevel = level; };
  LogType getFilterLevel() const { return filterLevel; };
};
  

} // namespace log
#endif