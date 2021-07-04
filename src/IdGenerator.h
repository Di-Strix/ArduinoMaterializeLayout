#include <Arduino.h>

#ifndef _MATERIALIZE_LAYOUT_ID_GENERATOR_H_
#define _MATERIALIZE_LAYOUT_ID_GENERATOR_H_

class IdGenerator
{
private:
  IdGenerator(){};
  IdGenerator(const IdGenerator &);
  IdGenerator &operator=(const IdGenerator &);

  size_t counter = 0;

public:
  static IdGenerator &Instance()
  {
    static IdGenerator idGenerator;
    return idGenerator;
  }

  size_t getId() { return this->counter += 1; }
};

#endif //_MATERIALIZE_LAYOUT_ID_GENERATOR_H_