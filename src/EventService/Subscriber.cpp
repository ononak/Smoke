#include "Subscriber.hpp"
#include "EventService.hpp"
#include <sys/socket.h>
#include <unistd.h>

namespace es {
/**
 * @brief Construct a new Subscriber:: Subscriber object
 *
 * @param description
 */
Subscriber::Subscriber(std::string_view description)
    : name(description), core::ActiveObject<EventPtr>(description) {
  service = EventService::get();
}

/**
 * @brief Destroy the Subscriber:: Subscriber object
 *
 */
Subscriber::~Subscriber() { unsubscribe(); }

/**
 * @brief
 *
 */
void Subscriber::subscribe() {
  try {
    auto handler = std::bind(&Subscriber::onData, this, std::placeholders::_1);
    this->subscriptionToken = service->subscribe(handler);
    this->run();
  } catch (const std::exception &e) {
    std::cerr << "Failed to subscribe" << e.what() << '\n';
  }
}

/**
 * @brief
 *
 */
void Subscriber::unsubscribe() {
  try {
    service->unsubscribe(this->subscriptionToken);
  } catch (const std::exception &e) {
    std::cerr << "Failed to unsubscribe " << e.what() << '\n';
  }
}

} // namespace es