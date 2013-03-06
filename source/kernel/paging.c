#include <kernel.h>

uint32_t *frames; // <-- bitset of whether each 4kb chunk of physical memory is mapped
uint32_t nframes; // <-- number of 4kb chunks of physical memory

extern uint32_t placement_pointer;

page_directory_t* kernel_directory = 0;
page_directory_t *current_directory = 0;

// Phyiscal frame bitset utils

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

static uint32_t test_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

static uint32_t first_frame()
{
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
   {
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++)
           {
               uint32_t toTest = 0x1 << j;
               if ( !(frames[i]&toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }
}

///

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
  if (page->frame != 0)
    return;

  uint32_t idx = first_frame();

  if (idx == (uint32_t)-1)
    PANIC("No free frames");

  set_frame(idx * 0x1000);
  page->present = 1;
  page->rw = (is_writeable) ? 1 : 0;
  page->user = (is_kernel) ? 0 : 1;
  page->frame = idx;
}

void free_frame(page_t *page)
{
  uint32_t frame;
  if (!(frame = page->frame))
    return;

  clear_frame(frame);
  page->frame = 0x0;
}

void init_paging()
{
  uint32_t mem_end_page = 0x1000000;

  nframes = mem_end_page / 0x1000;
  frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  kernel_directory = (page_directory_t*)kmalloc_aligned(sizeof(page_directory_t));
  memset(kernel_directory, 0, sizeof(page_directory_t));
  current_directory = kernel_directory;

  int i = 0;
  for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
    get_page(i, 1, kernel_directory);

  i = 0;
  while(i < placement_pointer + 0x1000)
  {
    alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    i += 0x1000;
  }

  for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
    alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

  register_interrupt_handler(14, &page_fault);

  switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
  current_directory = dir;
  asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
  uint32_t cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t* get_page(uint32_t address, int make, page_directory_t *dir)
{
  address /= 0x1000;
  uint32_t table_index = address / 1024;

  if (dir->tables[table_index])
    return &dir->tables[table_index]->pages[address % 1024];

  if (make)
  {
    uint32_t tmp;
    dir->tables[table_index] = (page_table_t*)kmalloc_aligned_physical(sizeof(page_table_t), &tmp);
    memset(dir->tables[table_index], 0, 0x1000);
    dir->tablesPhysical[table_index] = tmp | 0x7;
    return &dir->tables[table_index]->pages[address % 1024];
  }

  return 0;
}

void page_fault(registers_t *r)
{
  uint32_t cr2;
  asm volatile ("mov %%cr2, %0" : "=r" (cr2));

  int present   = !(r->err_code & 0x1); // Page not present
  int rw = r->err_code & 0x2;           // Write operation?
  int us = r->err_code & 0x4;           // Processor was in user-mode?
  int reserved = r->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  int id = r->err_code & 0x10;

  kprintf("Page fault at 0x%x, faulting address 0x%x", r->eip, cr2);
  kprintf("present: %d\nrw: %d\nus: %d\nreserved: %d\n", present, rw, us, reserved);

  ///kprint_stacktrace();

  //PANIC("");
  for(;;);
}
