#pragma once

#include <Arduino.h>
#include <functional>
#include <utility>

template <typename... Args>
class Subscription {
  typedef std::function<void(Args...)> Subscriber;
  typedef std::function<void(Subscription* target)> Unsubscriber;

  protected:
  Subscriber subscriber;
  Unsubscriber onUnsubscribe;

  public:
  /**
   * @brief Construct a new Subscription object
   *
   * @param subscriber subscriber function
   * @param onUnsubscribe Called when subscriber wants to unsubscribe
   */
  Subscription(Subscriber subscriber, Unsubscriber onUnsubscribe)
  {
    this->subscriber = subscriber;
    this->onUnsubscribe = onUnsubscribe;
  };

  /**
   * @brief Destroy the Subscription object and calls unsubscribe handler
   *
   */
  virtual ~Subscription()
  {
    this->onUnsubscribe(this);
  };

/**
 * @brief Calls subscriber with provided args
 * 
 * @param args 
 */
  void emit(Args... args)
  {
    this->subscriber(args...);
  }

/**
 * @brief Unsubscribes and deletes subscription object
 * 
 */
  void unsubscribe()
  {
    delete this;
  }
};
