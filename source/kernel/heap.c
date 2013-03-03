#include <kernel.h>

extern uint32_t end;
chunk_t *first_chunk = 0;
uint32_t placement_pointer = (uint32_t)&end;

void* kmalloc(size_t size)
{
  if (first_chunk)
  {

  }
  else
  {
    uint32_t alloced = placement_pointer;
    placement_pointer += size;

    kprintf("placement_pointer: %x  was: %x\n", placement_pointer, alloced);

    return (void*)alloced;
  }
}

void kfree(void* p)
{

}

void heap_install()
{

}
