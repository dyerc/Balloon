#ifndef MEMORY_H
#define MEMORY_H

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

// Heap

typedef struct chunk
{
  uint32_t magic;
  struct chunk *next, *prev;
  uint8_t allocated;
  uint32_t size;

} chunk_t;

// Zones not currently used... future thoughts
typedef struct
{
  uint32_t full;
  chunk_t *first;
  uint32_t offset;

} zone_t;

typedef struct
{
  chunk_t *first;
  uint32_t start;
  uint32_t end;
  uint32_t max;

} heap_t;

void* kmalloc(size_t size);
void kfree(void* p);

void* alloc(size_t size, uint8_t page_align, heap_t *heap);

void init_heap();

// Paging

typedef struct
{
  uint32_t present    : 1;
  uint32_t rw         : 1;
  uint32_t user       : 1;
  uint32_t accessed   : 1;
  uint32_t dirty      : 1;
  uint32_t unused     : 7; // MMU reserved
  uint32_t frame      : 20;

} page_t;

typedef struct
{
  page_t pages[1024];

} page_table_t;

typedef struct
{
  page_table_t *tables[1024];

  uint32_t tablesPhysical[1024];

  uint32_t physicalAddr;

} page_directory_t;

void init_paging();
void page_fault(registers_t *r);

page_t* get_page(uint32_t address, int make, page_directory_t *dir);
void switch_page_directory(page_directory_t *dir);

void* kmalloc(size_t size);
void* kmalloc_raw(size_t size, int align, uint32_t *phys);
void* kmalloc_aligned(size_t size);
void* kmalloc_physical(size_t size, uint32_t *phys);
void* kmalloc_aligned_physical(size_t size, uint32_t *phys);

#endif
