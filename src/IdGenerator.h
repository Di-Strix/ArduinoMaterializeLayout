#include <Arduino.h>

#pragma once

class IdGenerator {
  private:
  IdGenerator() {};
  IdGenerator(const IdGenerator&);
  IdGenerator& operator=(const IdGenerator&);

  size_t counter = 0;

  public:
  static IdGenerator& Instance()
  {
    static IdGenerator idGenerator;
    return idGenerator;
  }

  size_t getId() { return this->counter += 1; }
};
