#include <kernel.h>

#define HEAP_MAGIC  0xba110017;

extern uint32_t end;
chunk_t *first_chunk = 0;
uint32_t placement_pointer = (uint32_t)&end;



void* kmalloc(size_t size)
{
  return kmalloc_raw(size, 0, 0);
}

void* kmalloc_raw(size_t size, int align, uint32_t *phys)
{
  if (0)
  {

  }
  else
  {
    if (align == 1 && (placement_pointer & 0xFFFFF000))
    {
      // We need to page align it
      placement_pointer &= 0xFFFFF000;
      placement_pointer += 0x1000;
    }

    if (phys)
      *phys = placement_pointer;

    uint32_t alloced = placement_pointer;
    placement_pointer += size;

    return (void*)alloced;
  }
}

void* kmalloc_aligned(size_t size)
{
  return kmalloc_raw(size, 1, 0);
}

void* kmalloc_physical(size_t size, uint32_t *phys)
{
  return kmalloc_raw(size, 0, phys);
}

void* kmalloc_aligned_physical(size_t size, uint32_t *phys)
{
  return kmalloc_raw(size, 1, phys);
}

void kfree(void* p)
{

}

void heap_install()
{
  // Create heap_t

  // Create one large chunk of entire heap size
}

void* alloc(size_t size, uint8_t page_align, heap_t *heap)
{
  // Find smallest chunk that will fit size

  // No such chunk, expand heap to allow this

  // Create new free chunk after size is alloced

  // Write data to allocated chunk
    // chunk->next = newchunk->next;

  // return addr
}
