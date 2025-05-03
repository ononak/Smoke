
#include "Event.hpp"
#include <cstring>

namespace es {

Event::Event(const char *data, unsigned int length) {
  auto data_ptr = std::shared_ptr<char>(new char[std::strlen(data) + 1], std::default_delete<char[]>());;
  memcpy(data_ptr.get(), data, length);
  _event_data = std::make_tuple(data_ptr, length);
}

Event::Event() { _event_data = std::make_tuple(nullptr, 0); };

Event::~Event(){};

const EventData &Event::get_event_data() const { return _event_data; }
} // namespace es