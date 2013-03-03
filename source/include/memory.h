#ifndef MEMORY_H
#define MEMORY_H



typedef struct chunk {
  struct chunk *next, *prev;
  uint32_t allocated : 1;
  uint32_t size : 31;

} chunk_t;

// Zones not currently used... future thoughts
typedef struct {
  uint32_t full;
  chunk_t *first;
  uint32_t offset;

} zone_t;

void* kmalloc(size_t size);
void kfree(void* p);

#endif
