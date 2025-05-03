#include "EventService.hpp"
#include <iostream>

namespace es {

std::shared_ptr<EventService> EventService::get() {

  std::call_once(_onceFlag, []() {
    instance = std::shared_ptr<EventService>(new EventService());
  });

  return instance;
}


void EventService::run() {
  while (true)
    std::cin.get();
}

} // namespace es