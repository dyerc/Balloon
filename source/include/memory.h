#ifndef MEMORY_H
#define MEMORY_H

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define KHEAP_END   0x02000000

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

uint32_t kmalloc(size_t size);
uint32_t kmalloc_raw(size_t size, int align, uint32_t *phys);
uint32_t kmalloc_aligned(size_t size);
uint32_t kmalloc_physical(size_t size, uint32_t *phys);
uint32_t kmalloc_aligned_physical(size_t size, uint32_t *phys);

void * __attribute__ ((malloc)) malloc(size_t size);
void * __attribute__ ((malloc)) realloc(void *ptr, size_t size);
void * __attribute__ ((malloc)) calloc(size_t nmemb, size_t size);
void * __attribute__ ((malloc)) valloc(size_t size);
void free(void *ptr);

#endif
