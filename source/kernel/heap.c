#include <kernel.h>

#define HEAP_MAGIC  0xba110017;

extern uint32_t end;
chunk_t *first_chunk = 0;
uint32_t placement_pointer = (uint32_t)&end;

heap_t *kernel_heap = 0;

void* kmalloc(size_t size)
{
  return kmalloc_raw(size, 0, 0);
}

void* kmalloc_raw(size_t size, int align, uint32_t *phys)
{
  if (kernel_heap != 0)
  {
    return alloc(size, align, kernel_heap);
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

void init_heap()
{
  heap_t* heap = (heap_t*)kmalloc(sizeof(heap_t));
  uint32_t start = KHEAP_START;

  //ASSERT(start % 0x1000 == 0); // start = 0xC0000000
  //ASSERT(); // initial size = 0x100000

  if (start & 0xFFFFF000 != 0)
  {
    start &= 0xFFFFF000;
    start += 0x1000;
  }

  heap->start = start;
  heap->end = heap->start + KHEAP_INITIAL_SIZE;
  heap->max = heap->end + KHEAP_INITIAL_SIZE;
  //heap->supervisor = 0;
  //heap->readonly = 0;

  chunk_t* chunk = (chunk_t*)heap->start;
  chunk->size = heap->end - heap->start;
  chunk->magic = HEAP_MAGIC;
  chunk->next = chunk->prev = 0;
  chunk->allocated = 0;

  heap->first = chunk;

  kernel_heap = heap;
}

chunk_t* find_closest_chunk(uint32_t size, uint8_t page_align, heap_t *heap)
{
  chunk_t *current_chunk = heap->first;
  while(current_chunk)
  {
    if (current_chunk->allocated != 0)
    {
      current_chunk = current_chunk->next;
      continue;
    }

    if (page_align)
    {

    }
    else
    {
      if (current_chunk->size >= size)
        break;
    }

    current_chunk = current_chunk->next;
  }

  return current_chunk;
}

void* alloc(size_t size, uint8_t page_align, heap_t *heap)
{
  // Find smallest chunk that will fit size
  uint32_t required_size = size + sizeof(chunk_t);
  chunk_t* chunk = find_closest_chunk(required_size, page_align, heap);

  if (chunk)
  {
    if (chunk->size > required_size + sizeof(chunk_t)) // Do we have the space for a whole new chunk?
    {
      // We need to create a new chunk
      chunk_t *nextchunk = chunk + required_size;
      nextchunk->allocated = 0;
      nextchunk->size = chunk->size - required_size;
      nextchunk->prev = chunk;
      chunk->next = nextchunk;
      nextchunk->next = 0;
      nextchunk->magic = HEAP_MAGIC;
    }

    //ASSERT(chunk->magic == HEAP_MAGIC);
    chunk->allocated = 1;

    return (chunk + sizeof(chunk_t));
  }
  else
  {
    return 0;
  }

  // No such chunk, expand heap to allow this

  // Create new free chunk after size is alloced

  // Write data to allocated chunk
    // chunk->next = newchunk->next;

  // return addr
}
