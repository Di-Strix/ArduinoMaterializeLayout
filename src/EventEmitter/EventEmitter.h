#pragma once

#include <Arduino.h>

#include "./Subscription.h"

template <typename... Args>
class EventEmitter {
  protected:
  std::list<Subscription<Args...>*> subscribers;

  public:
  EventEmitter() = default;

  /**
   * @brief Destroys all the subscribers and the Event Emitter object
   * 
   */
  virtual ~EventEmitter()
  {
    for (auto subscriber : this->subscribers) {
      delete subscriber;
    }
  };

/**
 * @brief Subscribes to an event 
 * 
 * @param handler function that is called when event is fired
 * @return Subscription<Args...>* subscription control
 */
  Subscription<Args...>* subscribe(std::function<void(Args...)> handler)
  {
    auto subscription = new Subscription<Args...>(handler, [this](Subscription<Args...>* target) {
      this->subscribers.remove(target);
    });
    this->subscribers.push_back(subscription);

    return subscription;
  }

/**
 * @brief Fires an event
 * 
 * @param args 
 */
  void emit(Args... args)
  {
    for (auto subscriber : this->subscribers) {
      subscriber->emit(args...);
    }
  }
};
