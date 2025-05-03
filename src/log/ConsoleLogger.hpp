
#ifndef CONSOLELOGGER_HPP
#define CONSOLELOGGER_HPP

#include "../core/ActiveObject.hpp"
#include "LogFormater.hpp"
#include "ILogger.hpp"
#include <memory>

namespace logger {

using LogData = std::tuple<long long, std::string>;


class ConsoleLogger : public ILogger, public core::ActiveObject<LogData> {

private:
  static std::shared_ptr<ILogger> _instance;
  static std::mutex _mutex_creation;
  std::mutex _mutex_processing;
  DefaultFormater _formater;
   
public:
  static std::shared_ptr<ILogger> get();
  void print(LogType type, const std::string &message) override;
  void run() override;
  void shutdown() override;

protected:
  virtual void process(const LogData &message) override;
  ConsoleLogger(){};
};

} // namespace logger

#endif