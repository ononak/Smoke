#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <string_view>

namespace es {

/**
 * @brief
 *
 */
class Token {
  template <typename T> friend class Signal;
  friend bool operator==(const Token &a, const Token &b);
  friend bool operator<(const Token &a, const Token &b);

public:
  Token() = default;
  /**
   * @brief Destroy the Token object
   *
   */
  virtual ~Token() = default;

  /**
   * @brief Construct a new Token object
   *
   */
  Token(const Token &) = default;

private:
  int _identifier;
  std::string _keyword;

  /**
   * @brief Construct a new Token object
   *
   * @param id
   * @param key
   */
  Token(int id, const std::string_view &key)
      : _identifier(id), _keyword(key) {};
};

/**
 * @brief Interface definition for Signaling
 *
 * @tparam T
 */
template <typename T> class ISignal {
public:
  typedef std::function<void(const T &)> function_type;
  typedef std::map<Token, function_type> function_container_type;

public:
  virtual ~ISignal() {}
  virtual const Token connect(function_type slot,
                              const std::string &keyword = "") noexcept = 0;
  virtual void disconnect(const Token &Token) noexcept = 0;
  virtual void disconnect_all() noexcept = 0;
  virtual void operator()(const T &value) = 0;
  virtual size_t size() const = 0;
};

/**
 * @brief Allow registering functions that could be invoked when data become
 * available
 *
 * @tparam T
 */
template <typename T> class Signal : public ISignal<T> {

public:
  /**
   * @brief Construct a new Signal object
   *
   */
  Signal() {}

  /**
   * @brief Destroy the Signal object
   *
   */
  virtual ~Signal() {
    std::scoped_lock lock(_mutex);
    _slots.clear();
  }

  /**
   * @brief
   *
   * @param slot
   * @param keyword
   * @return const Token
   */
  [[nodiscard]] virtual const Token
  connect(typename ISignal<T>::function_type slot,
          const std::string &keyword = "") noexcept override {
    std::scoped_lock lock(_mutex);
    auto key = Token(_counter++, keyword);
    _slots.insert({key, slot});
    return key;
  }

  /**
   * @brief
   *
   * @param Token
   */
  virtual void disconnect(const Token &Token) noexcept override {
    std::scoped_lock lock(_mutex);
    _slots.erase(Token);
  }

  /**
   * @brief
   *
   */
  virtual void disconnect_all() noexcept override {
    std::scoped_lock lock(_mutex);
    _slots.clear();
  }

  /**
   * @brief
   *
   * @param value
   */
  virtual void operator()(const T &value) override {
    std::scoped_lock lock(_mutex);
    std::ranges::for_each(_slots, [&value](auto &sl) {
      try {
        sl.second(value);
      } catch (const std::exception &e) {
        std::cerr << "Callback threw an exception: " << e.what() << '\n';
      } catch (...) {
        std::cerr << "Callback threw an unknown exception.\n";
      }
    });
  }

  /**
   * @brief
   *
   * @return size_t
   */
  [[nodiscard]] virtual size_t size() const override {
    std::shared_lock lock(_mutex);
    return _slots.size();
  }

private:
  inline static std::atomic<int> _counter = 0;
  mutable std::shared_mutex _mutex;
  typename ISignal<T>::function_container_type _slots;

  Signal(const Signal &) = delete;
  Signal(Signal &&) = delete;
  Signal &operator=(const Signal &) = delete;
};


} // namespace es

#endif //
