#include "LogFormater.hpp"

namespace logger {
std::string _SOURCE(const char *filec, int linec, const char *funcc) {
  const std::string sp(":");
  std::string file(filec);
  std::string line(std::to_string(linec));
  std::string function(funcc);

  return sp + line + sp + function + sp + file + file;
};

} // namespace logger 