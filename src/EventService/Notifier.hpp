
#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include "Signal.hpp"


namespace es {

/**
 * @brief Notification class
 * Delivers published data to the subscribers
 *
 * @tparam T
 */
template <typename T> class Notifier : protected Signal<T> {
public:
  using subscriber_function = typename Signal<T>::function_type;

public:
  Notifier() : Signal<T>() {}

  /**
   * @brief Subscribe to notifications.
   * @param on_signal The function to be called when data is published.
   * @return const Token A unique token representing the subscription.
   */
  [[nodiscard]] const Token subscribe(subscriber_function on_signal) {
    return this->connect(on_signal, "");
  }

  /**
   * @brief Unsubscribe from notifications.
   * @param token The token representing the subscription to be removed.
   */
  void unsubscribe(const Token &token) { return this->disconnect(token); }

  /**
   * @brief Unsubscribe all subscribers.
   */
  void unsubscribe_all() { return this->disconnect_all(); }

  /**
   * @brief Publish data to all subscribers.
   * @param data The data to be published.
   */
  void publish(const T &data) { this->operator()(data); }

  /**
   * @brief Get the number of subscribers.
   * @return int The number of active subscribers.
   */
  std::size_t nof_subscriber() { return this->size(); }
};

} // namespace es

#endif