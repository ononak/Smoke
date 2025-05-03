#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include "../core/ActiveObject.hpp"
#include "Event.hpp"
#include "EventService.hpp"
#include <memory>
#include <queue>

namespace es {
class Subscriber : public core::ActiveObject<EventPtr> {
private:
  std::queue<std::shared_ptr<Event>> _messages;

protected:
  std::string name;
  Token subscriptionToken;
  std::shared_ptr<EventService> service;

public:
  Subscriber(std::string_view description = "");
  virtual ~Subscriber();
  /**
   * @brief Subscribes to events from the EventService.
   * Binds the onData method to the EventService and starts the event processing
   * loop.
   */
  void subscribe();
  /**
   * @brief Unsubscribes from the EventService.
   * Uses the stored subscription token to remove the subscription.
   */
  void unsubscribe();

protected:
  /**
   * @brief Processes an event.
   * This method must be implemented by derived classes.
   * @param event The event to process.
   */
  virtual void process(const EventPtr &event) = 0;
};
} // namespace es
#endif