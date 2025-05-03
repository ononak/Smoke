
#include "ConsoleLogger.hpp"

namespace logger {

std::shared_ptr<ILogger> ConsoleLogger::_instance;
std::mutex ConsoleLogger::_mutex_creation;

std::shared_ptr<ILogger> ConsoleLogger::get() {
  std::lock_guard<std::mutex> lock(_mutex_creation);
  if (_instance == nullptr) {
    _instance = std::shared_ptr<ILogger>(new ConsoleLogger());
  }
  return _instance;
}

void ConsoleLogger::print(LogType type, const std::string &message) {
  // call log formatter first

  long long milliseconds;
  std::string log_type;
  {
    std::lock_guard<std::mutex> lock(_mutex_processing);
    auto now = std::chrono::system_clock::now().time_since_epoch();
    milliseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(now).count();
  }
  /* add thread*/
  std::stringstream ss;
  ss << std::this_thread::get_id();

  onData({milliseconds, "<<" + ss.str() + ">> " + message});
}
 
void ConsoleLogger::process(const LogData &message) {
  std::lock_guard<std::mutex> lock(_mutex_processing);
  // std::cout << std::get<0>(message)<< " : " <<std::get<1>(message) <<
  // std::endl;
  std::cout << _formater(std::get<1>(message)) << std::endl;
}

void ConsoleLogger::run() { core::ActiveObject<LogData>::run(); }

void ConsoleLogger::shutdown() { core::ActiveObject<LogData>::shutdown(); }

} // namespace logger