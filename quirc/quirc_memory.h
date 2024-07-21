#ifndef QUIRC_MEMORY_H_
#define QUIRC_MEMORY_H_

#include "esp_heap_caps.h"

inline void* quirc_malloc(const size_t size) {
  return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}

#endif // QUIRC_MEMORY_H_