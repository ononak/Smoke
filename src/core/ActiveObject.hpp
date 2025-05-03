#ifndef ACTIVEOBJECT_HPP
#define ACTIVEOBJECT_HPP

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string_view>
#include <thread>

namespace core {

template <typename T> class ActiveObject {
private:
  std::queue<T> _data_store;
  std::mutex _mutex;
  std::condition_variable _condition;
  std::atomic_flag _shutdown = ATOMIC_FLAG_INIT;
  std::thread _thread;
  std::string _name;

public:
  ActiveObject(std::string_view description = "");
  virtual ~ActiveObject();

protected:
  virtual void process(const T &data) = 0;
  virtual void onData(const T &data);
  virtual void run();
  virtual void shutdown();

public:
  virtual void dataLoop();
};

template <typename T>
ActiveObject<T>::ActiveObject(std::string_view description)
    : _name(description){};

template <typename T> ActiveObject<T>::~ActiveObject() { shutdown(); };

template <typename T>
void ActiveObject<T>::onData(const T &data) {
  std::scoped_lock lock(_mutex);
  _data_store.emplace(data);
  _condition.notify_one();
}

template <typename T> void ActiveObject<T>::run() {
  std::scoped_lock lock(_mutex);
  _thread = std::thread(std::mem_fn(&ActiveObject<T>::dataLoop), this);
}

template <typename T> void ActiveObject<T>::shutdown() {
  _shutdown.test_and_set();
  _condition.notify_all();
  _thread.join();
}

template <typename T> void ActiveObject<T>::dataLoop() {
  std::unique_lock<std::mutex> lock(_mutex);

  while (!_shutdown.test()) {
    _condition.wait(lock);
    for (; !_data_store.empty(); _data_store.pop()) {
      auto data = _data_store.front();
      try {
        this->process(data);
      } 
      catch (const std::exception &e) {
        std::cerr << "Error processing data: " << e.what() << '\n';
      }  catch (...) {
        std::cerr << "Subscriber " << _name
                  << " -> Unknown exception: " << std::endl;
      }
    }
  }
}

} // namespace core
#endif