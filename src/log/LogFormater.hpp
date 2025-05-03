#ifndef LOGFORMATTER_HPP
#define LOGFORMATTER_HPP

#include <memory>
#include <string>

namespace logger {

std::string _SOURCE(const char *filec, int linec, const char *funcc);
#define SOURCE() _SOURCE(__FILE__, __LINE__, __func__)

/**
 * @brief
 *
 */

template <typename T> class ILogFormater {
private:
  std::shared_ptr<T> _next_formater;

protected:
  const std::string separator = " : ";

public:
  ILogFormater() : _next_formater(new T){};
  virtual std::string operator()(const std::string &message = "") const {
    return (_next_formater != nullptr) ? _next_formater->operator()(message)
                                       : separator;
  }; 
};

template <> class ILogFormater<void> {
private:
  std::shared_ptr<void> _next_formater;

protected:
  const std::string separator = " : ";

public:
  ILogFormater() : _next_formater(nullptr){};
  virtual std::string operator()(const std::string &message = "") const {
    return "";
  };
};

/**
 * @brief
 *
 */
template <typename T> class DateTimeFormater : public ILogFormater<T> {
public:
  DateTimeFormater() : ILogFormater<T>(){};
  std::string operator()(const std::string &message) const override;
};

/**
 * @brief
 *
 */
template <typename T> class MessageFormater : public ILogFormater<T> {
public:
  MessageFormater() : ILogFormater<T>(){};
  std::string operator()(const std::string &message) const override;
};

/**
 * @brief
 *
 * @param message
 * @return std::string
 */
template <typename T>
std::string MessageFormater<T>::operator()(const std::string &message) const {

  return message + ILogFormater<T>::separator +
         ILogFormater<T>::operator()(message);
  ;
}

/**
 * @brief
 *
 * @param message
 * @return std::string
 */
template <typename T>
std::string DateTimeFormater<T>::operator()(const std::string &message) const {

  std::string date(__DATE__);
  std::string time(__TIME__);

  return date + ILogFormater<T>::separator + time + ILogFormater<T>::separator +
         ILogFormater<T>::operator()(message);
}

typedef DateTimeFormater<MessageFormater<void>> DefaultFormater;

} // namespace logger

#endif