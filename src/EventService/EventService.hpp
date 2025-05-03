#ifndef EVENT_SERVICE_HPP
#define EVENT_SERVICE_HPP
#pragma once

#include "Event.hpp"
#include "Notifier.hpp"
#include <memory>

namespace es {

using EventHandler = Notifier<EventPtr>::subscriber_function;

using EventChannel = std::string ;

/**
 * @brief Singleton service for managing and notifying subscribers about events.
 */class EventService : public Notifier<EventPtr> {
private:
  EventService() = default;

private:
  inline static std::once_flag _onceFlag;
  inline static std::shared_ptr<EventService> instance = nullptr;

public:
  /**
   * @brief Get the singleton instance of the EventService.
   * @return A shared pointer to the EventService instance.
   */
  static std::shared_ptr<EventService> get();
  virtual ~EventService() = default;
    /**
   * @brief Start the event processing loop.
   */
  void run();

private:
  EventService(const EventService &) = delete;
  EventService &operator=(const EventService &) = delete;
  EventService(EventService &&) = delete;
  EventService &operator=(EventService &&) = delete;
};

} // namespace es
#endif