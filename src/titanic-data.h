#ifndef TITANIC_STRUCTURE_H
#define TITANIC_STRUCTURE_H

#include <stdint.h>

struct titanic_entry {
  uint8_t survived;
  uint8_t sex;
  float   age;
  uint8_t class1;
  uint8_t class2;
  uint8_t class3;
};

extern const struct titanic_entry titanic_data[];
extern const uint32_t titanic_data_size;

#endif
