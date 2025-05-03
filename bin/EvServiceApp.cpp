

#include "../src/EventService/EventService.hpp"
#include "../src/EventService/Subscriber.hpp"
#include "../src/log/ConsoleLogger.hpp"
#include "log/ILogger.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

std::shared_ptr<logger::ILogger> logser;
std::mutex mymutex;

int received = 0;
class MySubscriber : public es::Subscriber {

public:
  MySubscriber(const std::string name) : es::Subscriber(name) {}
  void process(const std::shared_ptr<es::Event> &event) override {
    std::lock_guard<std::mutex> lock(mymutex);
    auto data = event.get()->get_event_data();
    /* initialize random seed: */
    srand(time(NULL));

    /* generate secret number between 1 and 10: */
    auto iSecret = rand() % 300 + 1;

    std::this_thread::sleep_for(std::chrono::milliseconds(iSecret));

    logser->print(logger::INFO, "data received with sleeping time of " + std::to_string(iSecret) + " ms");
  }
};

int main(int, char **) {

  auto now = std::chrono::system_clock::now();
  time_t tt = std::chrono::system_clock::to_time_t(now);

  std::string sa(ctime(&tt));

  std::cout << ctime(&tt) << std::endl;

  logser = logger::ConsoleLogger::get();
  logser->run();

  logger::LogType a = logger::LogType::INFO;

  std::vector<std::shared_ptr<MySubscriber>> mysubscribers;
  auto eventService = es::EventService::get();
  for (auto i = 0; i < 50; i++) {

    mysubscribers.push_back(
        std::shared_ptr<MySubscriber>(new MySubscriber("My Subscriber")));
    mysubscribers[i]->subscribe();
  }

  srand(time(NULL));

  for (auto i = 0; i < 10000; i++) {
    int iSecret = rand() % 1000 + 1;
    std::stringstream ss;
    ss << iSecret;

    auto ev = std::shared_ptr<es::Event>(new es::Event(ss.str().c_str(), 3));
    // std::cout << "pubish " <<i<< std::endl;
    eventService->publish(ev);
  }

  eventService->run();
}
